#ifndef XC_H
#define XC_H

#ifndef XCLIB
#define XCLIB
#endif // !XCLIB

#include <stddef.h>
#include <stdlib.h>

// ------------------------
// Utility macros
// ------------------------

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

typedef size_t isize;
typedef ptrdiff_t usize;

typedef float  f32;
typedef double f64;

typedef enum XCComparisonResult {
    XC_CR_EQUAL,
    XC_CR_LESS_THAN,
    XC_CR_GREATER_THAN
} XCComparisonResult;

typedef enum XCBool {
    False,
    True
} XCBool;

// ------------------------
// Memory allocation
// ------------------------

#ifdef XC_DEBUG_MEM

usize XCDebug_BlockSize(void *block);

void *XCDebug_malloc(usize size);
void *XCDebug_calloc(usize count, usize size);
void *XCDebug_realloc(void *block, usize size);
void XCDebug_free(void *block);

#define malloc XCDebug_malloc
#define calloc XCDebug_calloc
#define realloc XCDebug_realloc
#define free XCDebug_free

#endif

// ------------------------
// Utility functions
// ------------------------

// `XCComparator` for `int`
XCLIB XCComparisonResult xcCompare_int(int *a, int *b);
// `XCComparator` for `uint`
XCLIB XCComparisonResult xcCompare_uint(uint *a, uint *b);
// `XCComparator` for `i8`
XCLIB XCComparisonResult xcCompare_i8(i8 *a, i8 *b);
// `XCComparator` for `u8`
XCLIB XCComparisonResult xcCompare_u8(u8 *a, u8 *b);
// `XCComparator` for `i16`
XCLIB XCComparisonResult xcCompare_i16(i16 *a, i16 *b);
// `XCComparator` for `u16`
XCLIB XCComparisonResult xcCompare_u16(u16 *a, u16 *b);
// `XCComparator` for `i32`
XCLIB XCComparisonResult xcCompare_i32(i32 *a, i32 *b);
// `XCComparator` for `u32`
XCLIB XCComparisonResult xcCompare_u32(u32 *a, u32 *b);
// `XCComparator` for `i64`
XCLIB XCComparisonResult xcCompare_i64(i64 *a, i64 *b);
// `XCComparator` for `u64`
XCLIB XCComparisonResult xcCompare_u64(u64 *a, u64 *b);
// `XCComparator` for `isize`
XCLIB XCComparisonResult xcCompare_isize(isize *a, isize *b);
// `XCComparator` for `usize`
XCLIB XCComparisonResult xcCompare_usize(usize *a, usize *b);
// `XCComparator` for `f32`
XCLIB XCComparisonResult xcCompare_f32(f32 *a, f32 *b);
// `XCComparator` for `f64`
XCLIB XCComparisonResult xcCompare_f64(f64 *a, f64 *b);

// ------------------------
// Dynamic arrays
// ------------------------

// Dynamic array type
typedef struct XCArray {
    usize len;      // length
    usize cap;      // capacity
    usize unitSize; // size of a single element in bytes
    void *data;     // data pointer
} XCArray;

// A function that compares two values given their pointers
typedef XCComparisonResult (*XCComparator)(void *, void *);
// A function that destroys a value given its pointer
typedef void (*XCDestructor)(void *);

// Create a new `XCArray` that contains values of size `unitSize`.
// Use `reserve` to pre-allocate a certain number of slots.
XCLIB XCArray *xcArrayNew(usize unitSize, usize reserve);
// Append a value to an `XCArray`, the data is copied from `value` to the array.
// If the memory allocation fails `False` is returned.
XCLIB XCBool xcArrayAppend(XCArray *array, void *value);
// Get the pointer to a value in an `XCArray`. `index` may be negative.
// If `index` is outisde the array `NULL` will be returned.
XCLIB void *xcArrayGet(XCArray *array, isize index);
// Set a value in an `XCArray`, the previous value is passed to `itemDestroyFunc`.
// `index` may be negative and `itemDestroyFunc` may be `NULL`.
// If `index` is outisde the array `False` will be returned.
XCLIB XCBool xcArraySet(XCArray *array, void *value, isize index, XCDestructor itemDestroyFunc);
// Remove a value from an `XCArray`, the removed value is passed to `itemDestroyFunc`.
// `index` may be negative and `itemDestroyFunc` may be `NULL`.
// If `index` is outisde the array `False` will be returned.
XCLIB XCBool xcArrayRemove(XCArray *array, isize index, XCDestructor itemDestroyFunc);
// Swap two values in an `XCArray`. `index1` and `index2` may be negative.
// If `index1` or `index2` is outisde the array `False` will be returned.
XCLIB XCBool xcArraySwap(XCArray *array, isize index1, isize index2);
// Move a value in an `XCArray` from `from` to `to`. `from` and `to` may be negative.
// If `from` or `to` is outisde the array `False` will be returned.
XCLIB XCBool xcArrayMove(XCArray *array, isize from, isize to);
// Sorts an `XCArray` using `compareFunc`
XCLIB void xcArraySort(XCArray *array, XCComparator compareFunc);
// Iterates through the values of an `xcArray`.
// `value` is the previous value returned; to start an iteration pass `value` as `NULL`
// When the end is reached `NULL` is returned.
XCLIB void *xcArrayNext(XCArray *array, void *value);
// Counts the occurences of `value` in an `XCArray` using `compareFunc` to compare them.
// An occurrence is counted when `compareFunc` returns `XC_CR_EQUAL`.
XCLIB usize xcArrayCount(XCArray *array, void *value, XCComparator compareFunc);
// Removes all occurences of `value` in an `XCArray` using `compareFunc` to compare them.
// An occurrence is counted when `compareFunc` returns `XC_CR_EQUAL`.
// `itemDestroyFunc` may be `NULL`.
// Returns the number of occurrences removed.
XCLIB usize xcArrayRemoveAll(XCArray *array, void *value, XCComparator compareFunc, XCDestructor itemDestroyFunc);
// Frees an `XCArray` from memory. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayDestroy(XCArray *array, XCDestructor itemDestroyFunc);

#endif // !XC_H
