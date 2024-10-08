#ifndef XC_H
#define XC_H

#ifndef XCLIB
#define XCLIB
#endif // !XCLIB

#include <stddef.h>
#include <stdlib.h>

#ifdef XC_NO_STDBOOL

typedef int bool;
#define false 0
#define true 1

#else

#include <stdbool.h>

#endif // !XC_NO_STDBOOL

// ------------------------
// Utility macros
// ------------------------

// Get the offset in bytes of a field in a struct
#define xcFieldOffset(struct, field) (usize)(&(((struct *)NULL)->field))

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


// ------------------------
// Dynamic arrays
// ------------------------

// Dynamic array type.
typedef struct XCArray {
    XCMemBlock data; // data pointer
    usize unitSize;  // size of a single item in bytes
    usize len;       // number of items in the array
    usize cap;       // capacity, the maximum `len` reachable before a reallocation
} XCArray;

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

// === Creation & Initialization ===

// Initialize an `XCArray` that contains values of size `unitSize`.
// Use `reserve` to pre-allocate a certain number of slots.
XCLIB bool xcArrayInit(XCArray *array, usize unitSize, usize reserve);
// Initialize a new `XCArray` by copying the data of an existing array.
XCLIB bool xcArrayInitFromCopy(XCArray *array, usize unitSize, usize count, XCMemBlock data);
// Initialize a new `XCArray` by using the `data` argument as the `data` field.
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

// === Destruction ===

// Destroy an `XCArray`, the array itself is not freed. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayDestroy(XCArray *array, XCDestructor itemDestroyFunc);
// Free an `XCArray` from memory. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayFree(XCArray *array, XCDestructor itemDestroyFunc);

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

// Iterates through the values of an `xcArray`.
// `value` is the previous value returned; to start an iteration pass `value` as `NULL`.
// When the end is reached `NULL` is returned.
XCLIB XCRef xcArrayNext(XCArray *array, XCRef value);

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


// ------------------------
// Boolean array
// ------------------------

// A compacted boolean array (1 bit per boolean)
typedef u8 *XCBoolArray;
// A chunk of booleans in a boolean array.
typedef u8 XCBoolArrayChunk;

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


// -----------------------------
// Strings & string manipulation
// -----------------------------

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

// Initialize a `XCStrView` from a `XCStr`
XCLIB void xcStrViewInitFromXCStr(XCStrView *strView, XCStr *str);

// === Destruction ===

// Destroy an `XCStr`, does not free `str` itself.
XCLIB void xcStrDestroy(XCStr *str);
// Free an `XCStr` from memory.
XCLIB void xcStrFree(XCStr *str);

#endif // !XC_H
