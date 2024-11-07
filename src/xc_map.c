#include <string.h>
#include "xc.h"

#define MIN_MAP_CAP 8

// Map hash retrival strategy.
//
// The `cap` of the map can always be expressed as a power of 2.
// Given a hash of the key, the first index is given by `idx = h % cap`.
// If it is occupied the following indices are found as such:
//
// idx_2 = (idx + (2*idx + 1)) % n
// idx_3 = (idx + (2*idx + 1) * 2) % n
// idx_4 = (idx + (2*idx + 1) * 3) % n
// ...
// idx_n = (idx + (2*idx + 1) * (n - 1)) % n
//
// where `n` is the capacity of the map.
//
// This way all indices are checked but in a different order, depending on the starting index.
//
//
// XCMap structure:
//
// Inside `data` is stored an array of `_XCMapItem` that contains the actual keys and values
// Inside `ptrTable` there are pointers to the items in `data` and are organized using the key's hash.
//
// To minimize allocations `ptrTable` is attached to `data`.
//
// Ex. of map of size 8, given an `_XCMapItem` of 8 bytes and sizeof(void *) = 4 bytes
// # = 4 bytes
//
//     +--------------------+
//     |                    |
//  +--+--------------------+-----+
//  V  V                    |     |
// [##|##|##|##|##|##|##|##|#|#|#|#|#|#|#|#]
//  ^                       ^
//  data                    ptrTable
//
// The `data` array is always filled from left to right and no holes are left, when an item is deleted the last one
// added is moved in its place to fill the hole.
//
// Having the `data` and `ptrTable` separate allows less copying when a full map re-insertion is needed (ex. when
// deleting an item).

struct _XCMapItem {
    u32 hash;
    usize _keyValue;
};

static void _xcMapItemInit(_XCMapItem *item, u32 hash, XCRef key, XCRef value, XCMap *map);
static usize _xcMapItemGetSize(XCMap *map);
static XCRef _xcMapItemGetKey(_XCMapItem *item);
static XCRef _xcMapItemGetValue(_XCMapItem *item, XCMap *map);
static bool _xcMapReallocData(XCMap *map, usize newCap);
static void _xcMapAddNew(XCMap *map, u32 hash, XCRef key, XCRef value);
static void _xcMapReInsert(XCMap *map, _XCMapItem *item);
static bool _xcMapExpand(XCMap *map);
static void _xcMapShrink(XCMap *map);
static void _xcMapClearOnly(XCMap *map, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc);

// === _XCMapItem functions ===

static void _xcMapItemInit(_XCMapItem *item, u32 hash, XCRef key, XCRef value, XCMap *map) {
    item->hash = hash;
    memcpy(&item->_keyValue, key, map->keySize);
    memcpy(_xcMapItemGetValue(item, map), value, map->valueSize);
}

static usize _xcMapItemGetSize(XCMap *map) {
    usize valueSize = map->valueSize;
    usize keySize = map->keySize;
    usize alignment = xcMin_usize(valueSize, sizeof(usize));
    usize padding = alignment - keySize % alignment;
    return sizeof(_XCMapItem) - sizeof(usize) + keySize + valueSize + padding;
}

static XCRef _xcMapItemGetKey(_XCMapItem *item) {
    return &item->_keyValue;
}

static XCRef _xcMapItemGetValue(_XCMapItem *item, XCMap *map) {
    usize valueSize = map->valueSize;
    usize keySize = map->keySize;
    usize alignment = xcMin_usize(valueSize, sizeof(usize));
    usize padding = alignment - keySize % alignment;
    return xcRawOffset(&item->_keyValue, keySize + alignment);
}

// === Creation & Initialization ===

static bool _xcMapReallocData(XCMap *map, usize newCap) {
    usize itemSize = _xcMapItemGetSize(map);
    XCMemBlock newData;
    _XCMapItem **newPtrTable;
    // if the capacity is the same only re-insert the items
    if (newCap == map->cap) {
        newData = map->data;
        newPtrTable = map->ptrTable;
    } else {
        newData = realloc(map->data, itemSize * newCap + sizeof(_XCMapItem *) * newCap);
        if (!newData)
            return map->cap > newCap; // shrinking should always succeed
        newPtrTable = xcRawOffset(newData, itemSize * newCap);
        map->data = newData;
        map->ptrTable = newPtrTable;
        map->cap = newCap;
    }

    memset(newPtrTable, 0, sizeof(_XCMapItem *) * newCap);

    for (usize i = 0, n = map->len; i < n; i++) {
        _XCMapItem *item = xcRawOffset(newData, itemSize * i);
        _xcMapReInsert(map, item);
    }
    return true;
}

XCLIB bool xcMapInit(XCMap *map, XCHasher hashFunc, XCComparator compareFunc, usize keySize, usize valueSize) {
    map->hashFunc = hashFunc;
    map->compareFunc = compareFunc;
    map->keySize = keySize;
    map->valueSize = valueSize;

    map->data = NULL;
    map->ptrTable = NULL;
    map->len = 0;
    map->cap = 0;

    if (!_xcMapReallocData(map, MIN_MAP_CAP))
        return false;

    return true;
}

XCLIB XCMap *xcMapNew(XCHasher hashFunc, XCComparator compareKey, usize keySize, usize valueSize) {
    XCMap *map = malloc(sizeof(XCMap));
    if (!map)
        return NULL;

    if (!xcMapInit(map, hashFunc, compareKey, keySize, valueSize)) {
        free(map);
        return NULL;
    }
    return map;
}

// === Destruction ===

XCLIB void xcMapDestroy(XCMap *map, XCDestructor destroyKey, XCDestructor destroyValue) {
    _xcMapClearOnly(map, destroyKey, destroyValue);
    free(map->data);
}

XCLIB void xcMapFree(XCMap *map, XCDestructor destroyKey, XCDestructor destroyValue) {
    xcMapDestroy(map, destroyKey, destroyValue);
    free(map);
}

// === Item retrival ===

XCLIB XCRef xcMapGet(XCMap *map, XCRef key) {
    usize map_cap = map->cap;
    u32 hash = map->hashFunc(key);
    u32 mask = (u32)(map_cap - 1);
    usize idx = hash & mask;

    XCComparator compareFunc = map->compareFunc;
    _XCMapItem **ptrTable = map->ptrTable;
    for (int i = 0; i < map_cap; i++) {
        usize idx_i = (idx + (2*idx + 1) * i) & mask;
        _XCMapItem *item = ptrTable[idx_i];
        if (!item) // item not in map
            return NULL;
        if (item->hash != hash)
            continue;
        if (compareFunc(key, _xcMapItemGetKey(item)) != 0)
            continue;
        // Item found!
        return _xcMapItemGetValue(item, map);
    }
    // the map is full but the item was not found
    return NULL;
}

// === Item addition & Manipulation ===

// Add a key-value pair that is certainly not in the map
// The map must have at least a free slot
static void _xcMapAddNew(XCMap *map, u32 hash, XCRef key, XCRef value) {
    usize itemSize = _xcMapItemGetSize(map);
    _XCMapItem *item = xcRawOffset(map->data, itemSize * map->len);
    _xcMapItemInit(item, hash, key, value, map);
    _xcMapReInsert(map, item);
}

// Re-insert an item already inside `data`
static void _xcMapReInsert(XCMap *map, _XCMapItem *item) {
    usize map_cap = map->cap;
    u32 mask = (u32)(map_cap - 1);
    usize idx = item->hash & mask;

    _XCMapItem **ptrTable = map->ptrTable;

    for (int i = 0; i < map_cap; i++) {
        usize idx_i = (idx + (2*idx + 1) * i) & mask;
        if (!ptrTable[idx_i]) {
            ptrTable[idx_i] = item;
            break;
        }
    }
}

static bool _xcMapExpand(XCMap *map) {
    return _xcMapReallocData(map, map->cap * 2);
}

XCLIB bool xcMapAdd(XCMap *map, XCRef key, XCRef value) {
    usize map_cap = map->cap;
    u32 hash = map->hashFunc(key);
    u32 mask = (u32)(map_cap - 1);
    usize idx = hash & mask;

    XCComparator compareFunc = map->compareFunc;
    _XCMapItem **ptrTable = map->ptrTable;

    for (int i = 0; i < map_cap; i++) {
        usize idx_i = (idx + (2*idx + 1) * i) & mask;
        _XCMapItem *item = ptrTable[idx_i];
        if (!item) { // item not in map & idx_i is the first free available slot
            usize itemSize = _xcMapItemGetSize(map);
            item = xcRawOffset(map->data, itemSize * map->len);
            _xcMapItemInit(item, hash, key, value, map);
            ptrTable[idx_i] = item;
            return true;
        }
        if (item->hash != hash)
            continue;
        if (compareFunc(key, _xcMapItemGetKey(item)) != 0)
            continue;
        // item already in map
        return false;
    }
    // the map is full but the item was not found
    if (!_xcMapExpand(map))
        return false;
    // the values were shifted in the expansion, search again for the first free slot
    _xcMapAddNew(map, hash, key, value);
    return true;
}

XCLIB bool xcMapSet(XCMap *map, XCRef key, XCRef value, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc) {
    usize map_cap = map->cap;
    u32 hash = map->hashFunc(key);
    u32 mask = (u32)(map_cap - 1);
    usize idx = hash & mask;

    XCComparator compareFunc = map->compareFunc;
    _XCMapItem **ptrTable = map->ptrTable;

    for (int i = 0; i < map_cap; i++) {
        usize idx_i = (idx + (2*idx + 1) * i) & mask;
        _XCMapItem *item = ptrTable[idx_i];
        if (!item) { // item not in map & idx_i is the first free available slot
            usize itemSize = _xcMapItemGetSize(map);
            item = xcRawOffset(map->data, itemSize * map->len);
            _xcMapItemInit(item, hash, key, value, map);
            ptrTable[idx_i] = item;
            return true;
        }
        if (item->hash != hash)
            continue;
        if (compareFunc(key, _xcMapItemGetKey(item)) != 0)
            continue;
        // item already in map, substitute the old key and value with the new ones
        destroyKey(_xcMapItemGetKey(item));
        destroyValue(_xcMapItemGetValue(item, map));
        _xcMapItemInit(item, hash, key, value, map);
        return true;
    }
    // the map is full but the item was not found
    if (!_xcMapExpand(map))
        return false;
    // the values were shifted in the expansion, search again for the first free slot
    _xcMapAddNew(map, hash, key, value);
    return true;
}

// === Item removal ===

XCLIB bool xcMapDel(XCMap *map, XCRef key, XCDestructor destroyKey, XCDestructor destroyValue) {
    usize map_cap = map->cap;
    u32 hash = map->hashFunc(key);
    u32 mask = (u32)(map_cap - 1);
    usize idx = hash & mask;

    XCComparator compareFunc = map->compareFunc;
    _XCMapItem **ptrTable = map->ptrTable;
    for (int i = 0; i < map_cap; i++) {
        usize idx_i = (idx + (2*idx + 1) * i) & mask;
        _XCMapItem *item = ptrTable[idx_i];
        if (!item) // item not in map
            return false;
        if (item->hash != hash)
            continue;
        if (compareFunc(key, _xcMapItemGetKey(item)) != 0)
            continue;
        // Item found!

        if (destroyKey)
            destroyKey(_xcMapItemGetKey(item));
        if (destroyValue)
            destroyValue(_xcMapItemGetValue(item, map));

        usize itemSize = _xcMapItemGetSize(map);
        memcpy(item, xcRawOffset(map->data, itemSize * (map->len - 1)), itemSize);
        return true;
    }
    // the map is full but the item was not found
    return NULL;
}

static void _xcMapClearOnly(XCMap *map, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc) {
    if (!destroyKeyFunc && !destroyValueFunc)
        return;

    usize itemSize = _xcMapItemGetSize(map);
    XCMemBlock data = map->data;
    for (usize i = 0, n = map->len; i < n; i++) {
        _XCMapItem *item = xcRawOffset(data, itemSize * i);
        if (destroyKeyFunc)
            destroyKey(_xcMapItemGetKey(item));
        if (destroyValueFunc) {
            destroyValue(_xcMapItemGetValue(item, map));
        }
    }
}

XCLIB void xcMapClear(XCMap *map, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc) {
    _xcMapClearOnly(map, destroyKeyFunc, destroyValueFunc);
    map->len = 0;
    _xcMapReallocData(map, MIN_MAP_CAP);
}

// === Iteration ===

XCLIB XCRef xcMapNext(XCMap *map, XCRef key, XCRef *outValue) {
    if (!key && map->len == 0) {
        if (outValue)
            *outValue = NULL;
        return NULL;
    }
    if (!key) {
        if (outValue)
            *outValue = _xcMapItemGetValue((_XCMapItem *)map->data, map);
        return _xcMapItemGetKey((_XCMapItem *)map->data);
    }
    usize itemSize = _xcMapItemGetSize(map);
    key = xcRawOffset(key, itemSize);
    if ((usize)key - (usize)map->data > map->len * itemSize) {
        if (outValue)
            *outValue = NULL;
        return NULL;
    }
    if (outValue) {
        _XCMapItem *item = (_XCMapItem *)((usize)key - xcFieldOffset(_XCMapItem, _keyValue));
        *outValue = _xcMapItemGetValue(item, map);
    }
    return key;
}
