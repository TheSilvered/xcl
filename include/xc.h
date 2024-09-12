#ifndef XC_H
#define XC_H

#ifndef XCLIB
#define XCLIB
#endif // !XCLIB

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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

typedef size_t isize;
typedef ptrdiff_t usize;

typedef float  f32;
typedef double f64;

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
XCLIB int xcCompare_int(void *a, void *b);
// `XCComparator` for `uint`
XCLIB int xcCompare_uint(void *a, void *b);
// `XCComparator` for `i8`
XCLIB int xcCompare_i8(void *a, void *b);
// `XCComparator` for `u8`
XCLIB int xcCompare_u8(void *a, void *b);
// `XCComparator` for `i16`
XCLIB int xcCompare_i16(void *a, void *b);
// `XCComparator` for `u16`
XCLIB int xcCompare_u16(void *a, void *b);
// `XCComparator` for `i32`
XCLIB int xcCompare_i32(void *a, void *b);
// `XCComparator` for `u32`
XCLIB int xcCompare_u32(void *a, void *b);
// `XCComparator` for `i64`
XCLIB int xcCompare_i64(void *a, void *b);
// `XCComparator` for `u64`
XCLIB int xcCompare_u64(void *a, void *b);
// `XCComparator` for `isize`
XCLIB int xcCompare_isize(void *a, void *b);
// `XCComparator` for `usize`
XCLIB int xcCompare_usize(void *a, void *b);
// `XCComparator` for `f32`
XCLIB int xcCompare_f32(void *a, void *b);
// `XCComparator` for `f64`
XCLIB int xcCompare_f64(void *a, void *b);

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
// ret < 0 means that a < b
// ret > 0 means that a > b
// ret == 0 means that a == b
typedef int (*XCComparator)(void *a, void *b);
// A function that destroys a value given its pointer
typedef void (*XCDestructor)(void *);

// === Creation & Initialization ===

// Initialize an `XCArray` that contains values of size `unitSize`.
// Use `reserve` to pre-allocate a certain number of slots.
XCLIB bool xcArrayInit(XCArray *array, usize unitSize, usize reserve);
// Initialize a new `XCArray` by copying the data of an existing array.
XCLIB bool xcArrayInitFromCopy(XCArray *array, usize unitSize, usize count, void *data);
// Initialize a new `XCArray` by using the `data` argument as the `data` field.
// Note that `data` must be a pointer to a heap-allocated block as it may be reallocted.
XCLIB void xcArrayInitFromData(XCArray *array, usize unitSize, usize count, void *data);
// Create a new `XCArray` that contains values of size `unitSize`.
// Use `reserve` to pre-allocate a certain number of slots.
XCLIB XCArray *xcArrayNew(usize unitSize, usize reserve);
// Create a new `XCArray` by copying the data of an existing array.
XCLIB XCArray *xcArrayNewFromCopy(usize unitSize, usize count, void *data);
// Create a new `XCArray` by using the `data` argument as the `data` field.
// Note that `data` must be a pointer to a heap-allocated block as it may be reallocted.
XCLIB XCArray *xcArrayNewFromData(usize unitSize, usize count, void *data);

// === Destruction ===

// Destroys an `XCArray`, the array itself is not freed. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayDestroy(XCArray *array, XCDestructor itemDestroyFunc);
// Frees an `XCArray` from memory. All remaining items are passed to `itemDestroyFunc`.
// `itemDestroyFunc` may be `NULL`.
XCLIB void xcArrayFree(XCArray *array, XCDestructor itemDestroyFunc);

// === Item manipulation ===

// Append a value to an `XCArray`, the data is copied from `value` to the array.
// If the memory allocation fails `False` is returned.
XCLIB bool xcArrayAppend(XCArray *array, void *value);
// Get the pointer to a value in an `XCArray`. `index` may be negative.
// If `index` is outisde the array `NULL` will be returned.
XCLIB void *xcArrayGet(XCArray *array, isize index);
// Set a value in an `XCArray`, the previous value is passed to `itemDestroyFunc`.
// `index` may be negative and `itemDestroyFunc` may be `NULL`.
// If `index` is outisde the array `False` will be returned.
XCLIB bool xcArraySet(XCArray *array, void *value, isize index, XCDestructor itemDestroyFunc);
// Remove a value from an `XCArray`, the removed value is passed to `itemDestroyFunc`.
// `index` may be negative and `itemDestroyFunc` may be `NULL`.
// If `index` is outisde the array `False` will be returned.
XCLIB bool xcArrayRemove(XCArray *array, isize index, XCDestructor itemDestroyFunc);
// Swap two values in an `XCArray`. `index1` and `index2` may be negative.
// If `index1` or `index2` is outisde the array `False` will be returned.
XCLIB bool xcArraySwap(XCArray *array, isize index1, isize index2);
// Move a value in an `XCArray` from `from` to `to`. `from` and `to` may be negative.
// If `from` or `to` is outisde the array `False` will be returned.
XCLIB bool xcArrayMove(XCArray *array, isize from, isize to);
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

// -----------------------------
// Strings & string manipulation
// -----------------------------

// UTF-8 encoded string
typedef struct XCStr {
    char *data; // string data
    usize len;  // length in bytes of `data`
    usize cap;  // capacity in bytes of `data`
} XCStr;

typedef struct XCStrView {
    char *data;
    usize len;
} XCStrView;

// A type that can contain any Unicode Codepoint
typedef u32 XCCodepoint;

XCLIB bool xcStrInitEmpty(XCStr *str);
// Create a new `XCStr` from a NUL-terminated string, the value is copied
XCLIB bool xcStrInit(XCStr *str, const char *value);
// Create a new `XCStr` by copying another string
XCLIB XCStr *xcStrCopy(XCStr *value);

#endif // !XC_H
