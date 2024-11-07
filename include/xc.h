#ifndef XC_H
#define XC_H

#ifndef XCLIB
#define XCLIB
#endif // !XCLIB

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

// ------------------------
// Utility macros
// ------------------------

// Get the offset in bytes of a field in a struct
#define xcFieldOffset(struct_, field) ((usize)(&(((struct_ *)NULL)->field)))
// Offset `ptr` by `bytes` bytes. The result is of type `void *`
#define xcRawOffset(ptr, bytes) ((void *)((u8 *)(ptr) + (bytes)))
// Get the reference of an rvalue. The result is of type `type *`
#define xcIRef(type, value) ((type *)&(struct { type _; }){ ._ = (value) })

// ------------------------
// Type aliases
// ------------------------

typedef unsigned int       uint;
typedef signed char        i8;
typedef unsigned char      u8;
typedef short              i16;
typedef unsigned short     u16;
typedef long               i32;
typedef unsigned long      u32;
typedef long long          i64;
typedef unsigned long long u64;

typedef size_t usize;
typedef ptrdiff_t isize;

typedef float  f32;
typedef double f64;

// A reference to a value
typedef void *XCRef;
// A pointer to a block of memory
typedef void *XCMemBlock;

// A function that compares two values given their pointers.
// ret < 0 means that a < b
// ret > 0 means that a > b
// ret == 0 means that a == b
typedef int (*XCComparator)(XCRef a, XCRef b);
// A function that filters a given value.
// ret == true means that the value is a match
// ret == false means that the value is not a match
typedef bool (*XCFilter)(XCRef);
// A function that destroys a value given its pointer.
typedef void (*XCDestructor)(XCRef);
// A function that gets the hash of a value given its pointer.
typedef u32 (*XCHasher)(XCRef);

// ------------------------
// Memory allocation
// ------------------------

#ifdef XC_DEBUG_MEM

usize xcDebug_BlockSize(XCMemBlock block);

XCMemBlock xcDebug_malloc(usize size);
XCMemBlock xcDebug_calloc(usize count, usize size);
XCMemBlock xcDebug_realloc(XCMemBlock block, usize size);
void xcDebug_free(XCMemBlock block);

#define malloc(size) xcDebug_malloc(size)
#define calloc(count, size) xcDebug_calloc(count, size)
#define realloc(block, size) xcDebug_realloc(block, size)
#define free(block) xcDebug_free(block)

#endif


// ------------------------
// Utility functions
// ------------------------

// `XCComparator` for `int`.
XCLIB int xcCompare_int(XCRef a, XCRef b);
// `XCComparator` for `uint`.
XCLIB int xcCompare_uint(XCRef a, XCRef b);
// `XCComparator` for `i8`.
XCLIB int xcCompare_i8(XCRef a, XCRef b);
// `XCComparator` for `u8`.
XCLIB int xcCompare_u8(XCRef a, XCRef b);
// `XCComparator` for `i16`.
XCLIB int xcCompare_i16(XCRef a, XCRef b);
// `XCComparator` for `u16`.
XCLIB int xcCompare_u16(XCRef a, XCRef b);
// `XCComparator` for `i32`.
XCLIB int xcCompare_i32(XCRef a, XCRef b);
// `XCComparator` for `u32`.
XCLIB int xcCompare_u32(XCRef a, XCRef b);
// `XCComparator` for `i64`.
XCLIB int xcCompare_i64(XCRef a, XCRef b);
// `XCComparator` for `u64`.
XCLIB int xcCompare_u64(XCRef a, XCRef b);
// `XCComparator` for `isize`.
XCLIB int xcCompare_isize(XCRef a, XCRef b);
// `XCComparator` for `usize`.
XCLIB int xcCompare_usize(XCRef a, XCRef b);
// `XCComparator` for `f32`.
XCLIB int xcCompare_f32(XCRef a, XCRef b);
// `XCComparator` for `f64`.
XCLIB int xcCompare_f64(XCRef a, XCRef b);

// Minimum between two `int` values.
XCLIB int xcMin_int(int a, int b);
// Minimum between two `uint` values.
XCLIB uint xcMin_uint(uint a, uint b);
// Minimum between two `i8` values.
XCLIB i8 xcMin_i8(i8 a, i8 b);
// Minimum between two `u8` values.
XCLIB u8 xcMin_u8(u8 a, u8 b);
// Minimum between two `i16` values.
XCLIB i16 xcMin_i16(i16 a, i16 b);
// Minimum between two `u16` values.
XCLIB u16 xcMin_u16(u16 a, u16 b);
// Minimum between two `i32` values.
XCLIB i32 xcMin_i32(i32 a, i32 b);
// Minimum between two `u32` values.
XCLIB u32 xcMin_u32(u32 a, u32 b);
// Minimum between two `i64` values.
XCLIB i64 xcMin_i64(i64 a, i64 b);
// Minimum between two `u64` values.
XCLIB u64 xcMin_u64(u64 a, u64 b);
// Minimum between two `isize` values.
XCLIB isize xcMin_isize(isize a, isize b);
// Minimum between two `usize` values.
XCLIB usize xcMin_usize(usize a, usize b);
// Minimum between two `f32` values.
XCLIB f32 xcMin_f32(f32 a, f32 b);
// Minimum between two `f64` values.
XCLIB f64 xcMin_f64(f64 a, f64 b);
// Minimum between two values of any type.
// If they are equal `a` is returned.
XCLIB XCRef xcMin(XCRef a, XCRef b, XCComparator compareFunc);

// Maximum between two `int` values.
XCLIB int xcMax_int(int a, int b);
// Maximum between two `uint` values.
XCLIB uint xcMax_uint(uint a, uint b);
// Maximum between two `i8` values.
XCLIB i8 xcMax_i8(i8 a, i8 b);
// Maximum between two `u8` values.
XCLIB u8 xcMax_u8(u8 a, u8 b);
// Maximum between two `i16` values.
XCLIB i16 xcMax_i16(i16 a, i16 b);
// Maximum between two `u16` values.
XCLIB u16 xcMax_u16(u16 a, u16 b);
// Maximum between two `i32` values.
XCLIB i32 xcMax_i32(i32 a, i32 b);
// Maximum between two `u32` values.
XCLIB u32 xcMax_u32(u32 a, u32 b);
// Maximum between two `i64` values.
XCLIB i64 xcMax_i64(i64 a, i64 b);
// Maximum between two `u64` values.
XCLIB u64 xcMax_u64(u64 a, u64 b);
// Maximum between two `isize` values.
XCLIB isize xcMax_isize(isize a, isize b);
// Maximum between two `usize` values.
XCLIB usize xcMax_usize(usize a, usize b);
// Maximum between two `f32` values.
XCLIB f32 xcMax_f32(f32 a, f32 b);
// Maximum between two `f64` values.
XCLIB f64 xcMax_f64(f64 a, f64 b);
// Maximum between two values of any type.
// If they are equal `a` is returned.
XCLIB XCRef xcMax(XCRef a, XCRef b, XCComparator compareFunc);

// Hash function for `int`
XCLIB u32 xcHash_int(XCRef num);
// Hash function for `uint`
XCLIB u32 xcHash_uint(XCRef num);
// Hash function for `i8`
XCLIB u32 xcHash_i8(XCRef num);
// Hash function for `u8`
XCLIB u32 xcHash_u8(XCRef num);
// Hash function for `i16`
XCLIB u32 xcHash_i16(XCRef num);
// Hash function for `u16`
XCLIB u32 xcHash_u16(XCRef num);
// Hash function for `i32`
XCLIB u32 xcHash_i32(XCRef num);
// Hash function for `u32`
XCLIB u32 xcHash_u32(XCRef num);
// Hash function for `i64`
XCLIB u32 xcHash_i64(XCRef num);
// Hash function for `u64`
XCLIB u32 xcHash_u64(XCRef num);
// Hash function for `isize`
XCLIB u32 xcHash_isize(XCRef num);
// Hash function for `usize`
XCLIB u32 xcHash_usize(XCRef num);

// ------------------------
// Dynamic arrays
// ------------------------

// Dynamic ordered array type.
typedef struct XCArray {
    XCMemBlock data; // data pointer
    usize unitSize;  // size of a single item in bytes
    usize len;       // number of items in the array
    usize cap;       // capacity, the maximum `len` reachable before a reallocation
} XCArray;

// View into a dynamic array, the data is not owned.
typedef struct XCArrayView {
    XCMemBlock data; // data pointer
    usize unitSize;  // size of a single item in bytes
    usize len;       // number of items in the array
} XCArrayView;

// === Creation & Initialization ===

// Initialize an `XCArray` that contains values of size `unitSize`.
// Use `reserve` to pre-allocate a certain number of slots.
XCLIB bool xcArrayInit(XCArray *array, usize unitSize, usize reserve);
// Initialize an `XCArray` by copying the data of an existing array.
// `count` refers to the number of items in `data`, `unitSize` to the size in bytes of a single item.
// The size of `data` is calculated as `unitSize * count`.
XCLIB bool xcArrayInitFromCopy(XCArray *array, usize unitSize, usize count, XCMemBlock data);
// Initialize an `XCArray` by using the `data` argument as the `data` field.
// `count` refers to the number of items in `data`, `unitSize` to the size in bytes of a single item.
// Note that `data` must be a pointer to a heap-allocated block as it may be reallocted.
XCLIB void xcArrayInitFromData(XCArray *array, usize unitSize, usize count, XCMemBlock data);
// Create a new `XCArray` that contains values of size `unitSize`.
// Use `reserve` to pre-allocate a certain number of slots.
XCLIB XCArray *xcArrayNew(usize unitSize, usize reserve);
// Create a new `XCArray` by copying the data of an existing array.
XCLIB XCArray *xcArrayNewFromCopy(usize unitSize, usize count, XCMemBlock data);
// Create a new `XCArray` by using the `data` argument as the `data` field.
// Note that `data` must be a pointer to a heap-allocated block as it may be reallocted.
XCLIB XCArray *xcArrayNewFromData(usize unitSize, usize count, XCMemBlock data);

// Initialize an `XCArrayView` from an `XCArray` that references indices from `from` included to `to` excluded.
// If the indices are outside the array they will be clamped to fit.
XCLIB void xcArrayViewInit(XCArrayView *view, XCArray *array, isize from, isize to);
// Initialize an `XCArrayView` from a block of data.
// `count` refers to the number of items in `data`, `unitSize` to the size in bytes of a single item.
XCLIB void xcArrayViewInitFromData(XCArrayView *view, usize unitSize, usize count, XCMemBlock data);

// Create a new `XCArrayView` from an `XCArray` that references indices from `from` included to `to` excluded.
// If the indices are outside the array they will be clamped to fit.
XCLIB XCArrayView *xcArrayViewNew(XCArray *array, isize from, isize to);
// Create a new `XCArrayView` from a block of data.
// `count` refers to the number of items in `data`, `unitSize` to the size in bytes of a single item.
XCLIB XCArrayView *xcArrayViewNewFromData(usize unitSize, usize count, XCMemBlock data);

// Create a new `XCArrayView` from an `XCArray` that references indices from `from` included to `to` excluded.
// If the indices are outside the array they will be clamped to fit.
XCLIB XCArrayView xcArrayViewMake(XCArray *array, isize from, isize to);
// Create a new `XCArrayView` from a block of data.
// `count` refers to the number of items in `data`, `unitSize` to the size in bytes of a single item.
XCLIB XCArrayView xcArrayViewMakeFromData(usize unitSize, usize count, XCMemBlock data);

// === Destruction ===

// Destroy an `XCArray`, the array itself is not freed. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayDestroy(XCArray *array, XCDestructor itemDestroyFunc);
// Free an `XCArray` from memory. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayFree(XCArray *array, XCDestructor itemDestroyFunc);

// Free an `XCArrayView` from memory, no items will be destroyed as it does not own the memory.
XCLIB void xcArrayViewFree(XCArrayView *view);

// === Item retrival ===

// Get the pointer to a value in an `XCArray`. `index` may be negative.
// If `index` is outisde the array `NULL` will be returned.
XCLIB XCRef xcArrayGet(XCArray *array, isize index);
// Find the first item that matches `value` using `compareFunc`.
// `value` is passed as the first argument of `compareFunc`.
// If no match is found `NULL` is returned.
XCLIB XCRef xcArrayFind(XCArray *array, XCRef value, XCComparator compareFunc);
// Get a list of elements that match `value` using `compareFunc` to compare them.
// `outBuf` is set to the output array, it is heap-allocated and references to the matching objects are put into it.
// `value` is passed as the first argument of `compareFunc`.
// Return the number of matching objects. If `outBuf` is `NULL` the objects are only counted.
XCLIB usize xcArrayFindAll(XCArray *array, XCRef value, XCComparator compareFunc, XCRef **outBuf);
// Get a list of elements that match using `filterFunc`.
// `outBuf` is set to the output array, it is heap-allocated and references to the matching objects are put into it.
// Return the number of matching objects. If `outBuf` is `NULL` the objects are only counted.
XCLIB usize xcArrayFilter(XCArray *array, XCFilter filterFunc, XCRef **outBuf);

// Get the pointer to a value in an `XCArrayView`. `index` may be negative.
// If `index` is outisde the array `NULL` will be returned.
XCLIB XCRef xcArrayViewGet(XCArrayView *view, isize index);
// Find the first item that matches `value` using `compareFunc`.
// `value` is passed as the first argument of `compareFunc`.
// If no match is found `NULL` is returned.
XCLIB XCRef xcArrayViewFind(XCArrayView *view, XCRef value, XCComparator compareFunc);
// Get a list of elements that match `value` using `compareFunc` to compare them.
// `outBuf` is set to the output array, it is heap-allocated and references to the matching objects are put into it.
// `value` is passed as the first argument of `compareFunc`.
// Return the number of matching objects. If `outBuf` is `NULL` the objects are only counted.
XCLIB usize xcArrayViewFindAll(XCArrayView *view, XCRef value, XCComparator compareFunc, XCRef **outBuf);
// Get a list of elements that match using `filterFunc`.
// `outBuf` is set to the output array, it is heap-allocated and references to the matching objects are put into it.
// Return the number of matching objects. If `outBuf` is `NULL` the objects are only counted.
XCLIB usize xcArrayViewFilter(XCArrayView *view, XCFilter filterFunc, XCRef **outBuf);

// === Item addition ===

// Append a value to an `XCArray`, the data is copied from `value` to the array.
// If the memory allocation fails `false` is returned.
XCLIB bool xcArrayAppend(XCArray *array, XCRef value);
// Insert a value in an `XCArray`. `index` may be negative`.
// If `index` is outside the array or a memory allocation fails `false` is returned.
XCLIB bool xcArrayInsert(XCArray *array, XCRef value, isize index);
// Extend an `XCArray` with the data in `newData`.
// `newDataLength` is the number of items in `newData`, the array's `unitSize` is used to
// determine the size of the memory to copy.
XCLIB bool xcArrayExtend(XCArray *array, usize newDataLength, XCMemBlock newData);

// === Item manipulation ===

// Set a value in an `XCArray`, the previous value is passed to `itemDestroyFunc`.
// `index` may be negative and `itemDestroyFunc` may be `NULL`.
// If `index` is outisde the array `false` will be returned.
XCLIB bool xcArraySet(XCArray *array, XCRef value, isize index, XCDestructor itemDestroyFunc);
// Swap two values in an `XCArray`. `index1` and `index2` may be negative.
// If `index1` or `index2` is outisde the array `false` will be returned.
XCLIB bool xcArraySwap(XCArray *array, isize index1, isize index2);
// Move a value in an `XCArray` from `from` to `to`. `from` and `to` may be negative.
// If `from` or `to` is outisde the array `false` will be returned.
XCLIB bool xcArrayMove(XCArray *array, isize from, isize to);
// Sort an `XCArray` using `compareFunc` in ascending order.
XCLIB void xcArraySort(XCArray *array, XCComparator compareFunc);

// === Item removal ===

// Remove a value from an `XCArray`, the removed value is passed to `itemDestroyFunc`.
// `index` may be negative and `itemDestroyFunc` may be `NULL`.
// If `index` is outisde the array `false` will be returned.
XCLIB bool xcArrayDel(XCArray *array, isize index, XCDestructor itemDestroyFunc);
// Remove all values from an `XCArray`, the removed values will be passed do `itemDestroyFunc`
XCLIB void xcArrayClear(XCArray *array, XCDestructor itemDestroyFunc);
// Remove the first value that matches `value` from an `XCArray`, the removed value is passed to `itemDestroyFunc`.
// Return `true` if a value was removed and `false` otherwise.
XCLIB bool xcArrayRemove(XCArray *array, XCRef value, XCComparator compareFunc, XCDestructor itemDestroyFunc);
// Remove all occurences of `value` in an `XCArray` using `compareFunc` to compare them.
// An occurrence is counted when `compareFunc` returns `0`.
// `value` is passed as the first argument of `compareFunc`.
// `itemDestroyFunc` may be `NULL`.
// Return the number of occurrences removed.
XCLIB usize xcArrayRemoveAll(XCArray *array, XCRef value, XCComparator compareFunc, XCDestructor itemDestroyFunc);
// Remove all values that match with `filterFunc` in an `XCArray`.
// `itemDestroyFunc` may be `NULL`.
// Return the number of items removed.
XCLIB usize xcArrayRemoveFilter(XCArray *array, XCFilter filterFunc, XCDestructor itemDestroyFunc);

// === Iteration ===

// Iterates through the values of an `XCArray`.
// `value` is the previous value returned; to start an iteration pass `value` as `NULL`.
// When the end is reached `NULL` is returned.
XCLIB XCRef xcArrayNext(XCArray *array, XCRef value);

// Iterates through the values of an `XCArrayView`.
// `value` is the previous value returned; to start an iteration pass `value` as `NULL`.
// When the end is reached `NULL` is returned.
XCLIB XCRef xcArrayViewNext(XCArrayView *view, XCRef value);

// === Index manipulation ===

// Makes an index always positive, and returns it.
// If the index is outside the array `-1` is returned.
XCLIB isize xcArrayIdxCheck(XCArray *array, isize idx);
// Get the index given the address of a value in the array.
// If the address is outside of the array or it is not a multiple of `unitSize` a negative value is returned.
XCLIB isize xcArrayRefToIdx(XCArray *array, XCRef ptr);
// Assuming `array` is sorted find the index where `value` first appears or where it would need to be
// inserted to keep the list sorted.
// `value` is passed to `compareFunc` as the first argument.
XCLIB usize xcArrayBisect(XCArray *array, XCRef value, XCComparator compareFunc);
// Assuming `array` is sorted find the index after the last `value` appears or where it would need to be
// inserted to keep the list sorted.
// `value` is passed to `compareFunc` as the first argument.
XCLIB usize xcArrayBisectRight(XCArray *array, XCRef value, XCComparator compareFunc);
// `xcArrayBisect` but with the ability to specify a specific interval.
// `lo` may be in the range [0, array->len), and `hi` in the range [0, array->len].
// If `hi` or `lo` is outside the specified range `-1` is returned.
XCLIB isize xcArrayBisectEx(XCArray *array, XCRef value, XCComparator compareFunc, usize lo, usize hi);
// `xcArrayBisectRight` but with the ability to specify a specific interval.
// `lo` may be in the range [0, array->len), and `hi` in the range [0, array->len].
// If `hi` or `lo` is outside the specified range `-1` is returned.
XCLIB isize xcArrayBisectRightEx(XCArray *array, XCRef value, XCComparator compareFunc, usize lo, usize hi);

// Makes an index always positive, and returns it.
// If the index is outside the array view `-1` is returned.
XCLIB isize xcArrayViewIdxCheck(XCArrayView *view, isize idx);
// Get the index given the address of a value in the array view.
// If the address is outside of the array or it is not a multiple of `unitSize` a negative value is returned.
XCLIB isize xcArrayViewRefToIdx(XCArrayView *view, XCRef ptr);
// Assuming `array` is sorted find the index where `value` first appears or where it would need to be
// inserted to keep the list sorted.
// `value` is passed to `compareFunc` as the first argument.
XCLIB usize xcArrayViewBisect(XCArrayView *view, XCRef value, XCComparator compareFunc);
// Assuming `array` is sorted find the index after the last `value` appears or where it would need to be
// inserted to keep the list sorted.
// `value` is passed to `compareFunc` as the first argument.
XCLIB usize xcArrayViewBisectRight(XCArrayView *view, XCRef value, XCComparator compareFunc);
// `xcArrayBisect` but with the ability to specify a specific interval.
// `lo` may be in the range [0, array->len), and `hi` in the range [0, array->len].
// If `hi` or `lo` is outside the specified range `-1` is returned.
XCLIB isize xcArrayViewBisectEx(XCArrayView *view, XCRef value, XCComparator compareFunc, usize lo, usize hi);
// `xcArrayBisectRight` but with the ability to specify a specific interval.
// `lo` may be in the range [0, array->len), and `hi` in the range [0, array->len].
// If `hi` or `lo` is outside the specified range `-1` is returned.
XCLIB isize xcArrayViewBisectRightEx(XCArrayView *view, XCRef value, XCComparator compareFunc, usize lo, usize hi);


// ------------------------
// Boolean array
// ------------------------

// A chunk of booleans in a boolean array.
typedef u8 XCBoolArrayChunk;
// A compacted boolean array (1 bit per boolean)
typedef XCBoolArrayChunk *XCBoolArray;

// Gets the number of chunks needed for an `XCBoolArray` of a given length.
// This can be used to create an `XCBoolArray` on the stack.
// For example `XCBoolArrayChunk arr[xcBoolArraySize(10)]` creates a boolean array of length `10`.
#define xcBoolArraySize(length) ((length) / sizeof(XCBoolArrayChunk) + ((length) % sizeof(XCBoolArrayChunk) ? 1 : 0))

// Create a new `XCBoolArray` of length `length`. All values are initialized to `false`.
XCLIB XCBoolArray xcBoolArrayNew(usize length);
// Free an `XCBoolArray`.
XCLIB void xcBoolArrayFree(XCBoolArray array);
// Get the value at index `idx` of an `XCBooleanArray`. Negative indices are not allowed.
XCLIB bool xcBoolArrayGet(XCBoolArray array, usize idx);
// Set the value at index `idx` of an `XCBooleanArray`. Negative indices are not allowed.
XCLIB void xcBoolArraySet(XCBoolArray array, usize idx, bool value);
// Set all values of an `XCBooleanArray` to `value`.
XCLIB void xcBoolArraySetAll(XCBoolArray array, usize length, bool value);

// ------------------------
// Hash map
// ------------------------

typedef struct _XCMapItem _XCMapItem;

// Dynamic unordered hash map type.
typedef struct XCMap {
    XCMemBlock data;          // key-value pairs data pointer
    _XCMapItem **ptrTable;    // array of pointers to key-value pairs
    XCHasher hashFunc;        // function used to hash the keys
    XCComparator compareFunc; // function used to compare keys directly
    usize keySize;            // size in bytes of a key
    usize valueSize;          // size in bytes of a value
    usize len;                // number of key-value pairs in the map
    usize cap;                // capacity, the maximum `len` reachable before a reallocation
} XCMap;

// === Creation & Initialization ===

XCLIB bool xcMapInit(XCMap *map, XCHasher hashFunc, XCComparator compareFunc, usize keySize, usize valueSize);
XCLIB XCMap *xcMapNew(XCHasher hashFunc, XCComparator compareFunc, usize keySize, usize valueSize);

// === Destruction ===

XCLIB void xcMapDestroy(XCMap *map, XCDestructor destroyKey, XCDestructor destroyValue);
XCLIB void xcMapFree(XCMap *map, XCDestructor destroyKey, XCDestructor destroyValue);

// === Item retrival ===

XCLIB XCRef xcMapGet(XCMap *map, XCRef key);

// === Item addition & Manipulation ===

XCLIB bool xcMapAdd(XCMap *map, XCRef key, XCRef value);
XCLIB bool xcMapSet(XCMap *map, XCRef key, XCRef value, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc);

// === Item removal ===

// Deletes the key-value pair containig `key`.
// After this operation any reference to a key or value in the map becomes invalid as the values might have moved.
XCLIB bool xcMapDel(XCMap *map, XCRef key, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc);
XCLIB void xcMapClear(XCMap *map, XCDestructor destroyKeyFunc, XCDestructor destroyValueFunc);

// === Iteration ===

XCLIB XCRef xcMapNext(XCMap *map, XCRef key, XCRef *outValue);

// ------------------------
// Strings
// ------------------------

// A general string.
typedef struct XCStr {
    char *data; // string data, it is not NUL-terminated
    usize size; // size in bytes of `data`
    usize cap;  // capacity in bytes of `data`
} XCStr;

// A view into a general string.
typedef struct XCStrView {
    char *data; // pointer to the data, the memory is not owned
    usize size; // size in bytes of `data`
} XCStrView;

// === Creation & Initialization ===

// Initialize an `XCStr` from a NUL-terminated string, the value is copied.
XCLIB bool xcStrInit(XCStr *str, const char *value);
// Initialize an `XCStr` to be empty.
XCLIB bool xcStrInitEmpty(XCStr *str);
// Initialize an `XCStr` given its data and its size.
// The `data` must be a pointer to a heap-allocated block.
XCLIB void xcStrInitFromData(XCStr *str, char *data, usize size);
// Create a new `XCStr` from a NUL-terminated string, the value is copied.
XCLIB XCStr *xcStrNew(const char *value);
// Create a new empty `XCStr`.
XCLIB XCStr *xcStrNewEmpty(void);
// Create a new `XCStr` given its data and its size.
XCLIB XCStr *xcStrNewFromData(char *data, usize size);

// Initialize an `XCStrView` from a NUL-terminated string, the value is referenced, not copied.
XCLIB void xcStrViewInit(XCStrView *strView, const char *value);
// Create a new `XCStrView` from a NUL-terminated string, the value is copied.
XCLIB XCStrView xcStrViewMake(const char *value);
// Initialize a `XCStrView` from a `XCStr`
XCLIB void xcStrViewInitFromXCStr(XCStrView *strView, XCStr *str);

// === Destruction ===

// Destroy an `XCStr`, does not free `str` itself.
XCLIB void xcStrDestroy(XCStr *str);
// Free an `XCStr` from memory.
XCLIB void xcStrFree(XCStr *str);

// === Hashing ===

XCLIB u32 xcStrHash(XCRef str);

// === Comparison ===

XCLIB int xcStrCompare(XCRef str1, XCRef str2);

#endif // !XC_H
