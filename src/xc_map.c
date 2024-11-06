#include <string.h>
#include "xc.h"

#define MIN_MAP_CAP 8

/**
 * Map hash retrival strategy.
 *
 * The `cap` of the map can always be expressed as a power of 2.
 * Given a hash of the key, the first index is given by `idx = h % cap`.
 * If it is occupied the following indices are found as such:
 *
 * idx_2 = (idx + (2*idx + 1)) % n
 * idx_3 = (idx + (2*idx + 1) * 2) % n
 * idx_4 = (idx + (2*idx + 1) * 3) % n
 * ...
 * idx_n = (idx + (2*idx + 1) * (n - 1)) % n
 *
 * where `n` is the capacity of the map.
 *
 * This way all indices are checked but in a different order, depending on the starting index.
 *
 *
 * XCMap structure:
 *
 * Inside `data` is stored an array of `_XCMapItem` that contains the actual keys and values
 * Inside `ptrTable` there are pointers to the items in `data` and are organized using the key's hash.
 *
 * To minimize allocations `ptrTable` is attached to `data`.
 *
 * Ex. of map of size 8, given an `_XCMapItem` of 8 bytes and sizeof(void *) = 4 bytes
 * # = 4 bytes
 *
 *     +--------------------+
 *     |                    |
 *  +--+--------------------+-----+
 *  V  V                    |     |
 * [##|##|##|##|##|##|##|##|#|#|#|#|#|#|#|#]
 *  ^                       ^
 *  data                    ptrTable
 *
 * The `data` array is always filled from left to right and no holes are left, when an item is deleted the last one
 * added is moved in its place to fill the hole.
 *
 * Having the `data` and `ptrTable` separate allows less copying when a full map re-insertion is needed (ex. when
 * deleting an item).
 */

struct _XCMapItem {
    u32 hash;
    usize _keyValue;
};

static void _xcMapItemInit(_XCMapItem *item, u32 hash, XCRef key, XCRef value, usize keySize, usize valueSize);
static usize _xcMapItemGetSize(usize keySize, usize valueSize);
static XCRef _xcMapItemGetKey(_XCMapItem *item);
static XCRef _xcMapItemGetValue(_XCMapItem *item, usize keySize, usize valueSize);
static void _xcMapAddNew(XCMap *map, u32 hash, XCRef key, XCRef value);
static void _xcMapReInsert(XCMap *map, _XCMapItem *item);

static void _xcMapItemInit(_XCMapItem *item, u32 hash, XCRef key, XCRef value, usize keySize, usize valueSize) {
    item->hash = hash;
    memcpy(&item->_keyValue, key, keySize);
    memcpy(_xcMapItemGetValue(item, keySize, valueSize), value, valueSize);
}

static usize _xcMapItemGetSize(usize keySize, usize valueSize) {
    usize alignment = xcMin_usize(valueSize, sizeof(usize));
    usize padding = alignment - keySize % alignment;
    return sizeof(_XCMapItem) - sizeof(usize) + keySize + valueSize + padding;
}

static XCRef _xcMapItemGetKey(_XCMapItem *item) {
    return &item->_keyValue;
}

static XCRef _xcMapItemGetValue(_XCMapItem *item, usize keySize, usize valueSize) {
    usize alignment = xcMin_usize(valueSize, sizeof(usize));
    usize padding = alignment - keySize % alignment;
    return xcRawOffset(&item->_keyValue, keySize + alignment);
}

XCLIB bool xcMapInit(XCMap *map, XCHasher hashFunc, XCComparator compareFunc, usize keySize, usize valueSize) {
    usize itemSize = _xcMapItemGetSize(keySize, valueSize);

    // map->idxTable is attached at the end of map->data
    map->data = malloc(itemSize * MIN_MAP_CAP + sizeof(_XCMapItem *) * MIN_MAP_CAP);
    if (!map->data)
        return false;
    map->ptrTable = xcRawOffset(map->data, itemSize * MIN_MAP_CAP);

    map->hashFunc = hashFunc;
    map->compareFunc = compareFunc;
    map->keySize = keySize;
    map->valueSize = valueSize;
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

/*
XCLIB void xcMapDestroy(XCMap *map, XCDestructor destroyKey, XCDestructor destroyValue) {
    free(map->data);
}

XCLIB void xcMapFree(XCMap *map, XCDestructor destroyKey, XCDestructor destroyValue) {
    xcMapDestroy(map, destroyKey, destroyValue);
    free(map);
}
*/

// Add a key-value pair that is certainly not in the map
// The map must have at least a free slot
static void _xcMapAddNew(XCMap *map, u32 hash, XCRef key, XCRef value) {
    usize itemSize = _xcMapItemGetSize(map->keySize, map->valueSize);
    _XCMapItem *item = xcRawOffset(map->data, itemSize * map->len);
    _xcMapItemInit(item, hash, key, value, map->keySize, map->valueSize);
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

// Always expands the map, reguardless of current `len` and `cap`
static bool _xcMapExpand(XCMap *map) {
    usize itemSize = _xcMapItemGetSize(map->keySize, map->valueSize);
    usize newCap = map->cap * 2;
    XCMemBlock newData = realloc(map->data, itemSize * newCap + sizeof(_XCMapItem *) * newCap);
    if (!newData)
        return false;

    _XCMapItem **newPtrTable = xcRawOffset(newData, itemSize * newCap);

    memset(newPtrTable, 0, sizeof(_XCMapItem *) * newCap);
    map->data = newData;
    map->ptrTable = newPtrTable;
    map->cap = newCap;

    for (usize i = 0, n = map->len; i < n; i++) {
        _XCMapItem *item = xcRawOffset(newData, itemSize * i);
        _xcMapReInsert(map, item);
    }
    return true;
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
            usize itemSize = _xcMapItemGetSize(map->keySize, map->valueSize);
            item = xcRawOffset(map->data, itemSize * map->len);
            _xcMapItemInit(item, hash, key, value, map->keySize, map->valueSize);
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

XCLIB bool xcMapSet(XCMap *map, XCRef key, XCRef value, XCDestructor destroyKey, XCDestructor destroyValue) {
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
            usize itemSize = _xcMapItemGetSize(map->keySize, map->valueSize);
            item = xcRawOffset(map->data, itemSize * map->len);
            _xcMapItemInit(item, hash, key, value, map->keySize, map->valueSize);
            ptrTable[idx_i] = item;
            return true;
        }
        if (item->hash != hash)
            continue;
        if (compareFunc(key, _xcMapItemGetKey(item)) != 0)
            continue;
        // item already in map, substitute the old key and value with the new ones
        destroyKey(_xcMapItemGetKey(item));
        destroyValue(_xcMapItemGetValue(item, map->keySize, map->valueSize));
        _xcMapItemInit(item, hash, key, value, map->keySize, map->valueSize);
        return true;
    }
    // the map is full but the item was not found
    if (!_xcMapExpand(map))
        return false;
    // the values were shifted in the expansion, search again for the first free slot
    _xcMapAddNew(map, hash, key, value);
    return true;
}

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
        return _xcMapItemGetValue(item, map->keySize, map->valueSize);
    }
    // the map is full but the item was not found
    return NULL;
}
