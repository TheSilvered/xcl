#include "xc.h"
#include <string.h>
#include <stdio.h>

#define MIN_ARRAY_CAP 8
#define SORT_RUN_SIZE 32
#define STATIC_BUF_SIZE 64

static inline void *_xcArrayGetFast(XCArray *array, usize index);
static inline bool _xcArrayExpand(XCArray *array);
static inline void _xcArrayShrink(XCArray *array);
static inline void _xcArraySetFast(XCArray *array, void *value, usize index);
static inline void _xcArraySwapFast(XCArray *array, usize index1, usize index2);
static void _xcArrayMoveFast(XCArray *array, usize from, usize to);
static void _xcInsertionSort(XCArray *array, XCComparator compareFunc, usize start, usize end);
static void _xcInPlaceMergeSort(XCArray *array, XCComparator compareFunc, usize start, usize middle, usize end);
static void _xcMergeSort(u8 *tempBuf, XCArray *array, XCComparator compareFunc, usize start, usize middle, usize end);

// === Creation & Initialization ===

bool xcArrayInit(XCArray *array, usize unitSize, usize reserve) {
    array->data = malloc((MIN_ARRAY_CAP > reserve ? MIN_ARRAY_CAP : reserve) * unitSize);
    if (!array->data)
        return false;
    array->cap = reserve;
    array->len = 0;
    array->unitSize = unitSize;
    return true;
}

bool xcArrayInitFromCopy(XCArray *array, usize unitSize, usize count, void *data) {
    if (!xcArrayInit(array, unitSize, count))
        return false;
    memcpy(array->data, data, unitSize * count);
    array->len = count;
    return true;
}

void xcArrayInitFromData(XCArray *array, usize unitSize, usize count, void *data) {
    array->data = data;
    array->cap = count;
    array->len = count;
    array->unitSize = unitSize;
}

XCArray *xcArrayNew(usize unitSize, usize reserve) {
    XCArray *array = malloc(sizeof(XCArray));
    if (!array)
        return NULL;
    if (!xcArrayInit(array, unitSize, reserve)) {
        free(array);
        return NULL;
    }
    return array;
}

XCArray *xcArrayNewFromCopy(usize unitSize, usize count, void *data) {
    XCArray *array = malloc(sizeof(XCArray));
    if (!array)
        return NULL;
    if (!xcArrayInitFromCopy(array, unitSize, count, data)) {
        free(array);
        return NULL;
    }
    return array;
}

XCArray *xcArrayNewFromData(usize unitSize, usize count, void *data) {
    XCArray *array = malloc(sizeof(XCArray));
    if (!array)
        return NULL;
    xcArrayInitFromData(array, unitSize, count, data);
    return array;
}

// === Destruction ===

void xcArrayDestroy(XCArray *array, XCDestructor itemDestroyFunc) {
    if (!array)
        return;
    if (itemDestroyFunc) {
        for (void *v = xcArrayNext(array, NULL); v; v = xcArrayNext(array, v))
            itemDestroyFunc(v);
    }
    free(array->data);
}

void xcArrayFree(XCArray *array, XCDestructor itemDestroyFunc) {
    if (!array)
        return;
    xcArrayDestroy(array, itemDestroyFunc);
    free(array);
}

static inline void *_xcArrayGetFast(XCArray *array, usize index) {
    return (void *)((u8 *)array->data + index * array->unitSize);
}

// === Item retrival ===

void *xcArrayGet(XCArray *array, isize index) {
    index = xcArrayIdxCheck(array, index);
    if (index < 0)
        return NULL;
    return _xcArrayGetFast(array, (usize)index);
}

void *xcArrayFind(XCArray *array, void *value, XCComparator compareFunc) {
    for (void *v = xcArrayNext(array, NULL); v; v = xcArrayNext(array, v)) {
        if (compareFunc(v, value) == 0)
            return v;
    }
    return NULL;
}

usize xcArrayFindAll(XCArray *array, void *value, XCComparator compareFunc, void **outBuf) {
    XCArray foundArr;
    if (outBuf && !xcArrayInit(&foundArr, array->unitSize, 0)) {
        *outBuf = NULL;
        return 0;
    }
    *outBuf = array->data;
    usize count = 0;
    for (void *v = xcArrayNext(array, NULL); v; v = xcArrayNext(array, v)) {
        if (compareFunc(v, value) == 0) {
            if (outBuf && !xcArrayAppend(&foundArr, v))
                return count;
            count++;
        }
    }
    return count;
}

usize xcArrayFilter(XCArray *array, XCFilter filterFunc, void **outBuf) {
    XCArray foundArr;
    if (outBuf && !xcArrayInit(&foundArr, array->unitSize, 0)) {
        *outBuf = NULL;
        return 0;
    }
    *outBuf = array->data;
    usize count = 0;
    for (void *v = xcArrayNext(array, NULL); v; v = xcArrayNext(array, v)) {
        if (filterFunc(v) == 0) {
            if (outBuf && !xcArrayAppend(&foundArr, v))
                return count;
            count++;
        }
    }
    return count;
}

// === Item addition ===

static inline bool _xcArrayExpand(XCArray *array) {
    if (array->len < array->cap)
        return true;
    usize newCap = (usize)((double)array->cap * 1.5);
    void *newData = realloc(array->data, newCap * array->unitSize);
    if (!newData)
        return false;
    array->data = newData;
    array->cap = newCap;
    return true;
}

static inline void _xcArrayShrink(XCArray *array) {
    if (array->len >= array->cap / 4)
        return;
    usize newCap = array->cap / 2;
    if (newCap < MIN_ARRAY_CAP)
        return;
    void *newData = realloc(array->data, newCap * array->unitSize);
    if (!newData)
        return;
    array->data = newData;
    array->cap = newCap;
}

bool xcArrayAppend(XCArray *array, void *value) {
    if (!_xcArrayExpand(array))
        return false;
    memcpy((u8 *)array->data + array->len * array->unitSize, value, array->unitSize);
    array->len++;
    return true;
}

bool xcArrayInsert(XCArray *array, void *value, isize index) {
    index = xcArrayIdxCheck(array, index);
    if (index < 0)
        return false;
    if (!xcArrayAppend(array, value))
        return false;
    _xcArrayMoveFast(array, array->len - 1, index);
    return true;
}

// === Item manipulation ===

static inline void _xcArraySetFast(XCArray *array, void *value, usize index) {
    memcpy(_xcArrayGetFast(array, index), value, array->unitSize);
}

bool xcArraySet(XCArray *array, void *value, isize index, XCDestructor itemDestroyFunc) {
    void *arrayValue = xcArrayGet(array, index);
    if (!arrayValue)
        return false;
    if (itemDestroyFunc)
        itemDestroyFunc(arrayValue);
    memcpy(arrayValue, value, array->unitSize);
    return true;
}

static inline void _xcArraySwapFast(XCArray *array, usize index1, usize index2) {
    u8 *v1 = _xcArrayGetFast(array, index1);
    u8 *v2 = _xcArrayGetFast(array, index2);
    if (array->unitSize > STATIC_BUF_SIZE) {
        u8 temp;
        for (usize i = 0, n = array->unitSize; i < n; i++) {
            temp = v1[i];
            v1[i] = v2[i];
            v2[i] = temp;
        }
    } else {
        u8 staticBuf[STATIC_BUF_SIZE];
        usize unitSize = array->unitSize;
        memcpy(staticBuf, v1, unitSize);
        memcpy(v1, v2, unitSize);
        memcpy(v2, staticBuf, unitSize);
    }
}

bool xcArraySwap(XCArray *array, isize index1, isize index2) {
    index1 = xcArrayIdxCheck(array, index1);
    index2 = xcArrayIdxCheck(array, index2);
    if (index1 < 0 || index2 < 0)
        return NULL;
    _xcArraySwapFast(array, index1, index2);
    return true;
}

static void _xcArrayMoveFast(XCArray *array, usize from, usize to) {
    if (from == to)
        return;
    u8 staticBuf[STATIC_BUF_SIZE];
    usize unitSize = array->unitSize;
    if (from < to) {
        if (unitSize > STATIC_BUF_SIZE) {
            for (usize i = from; i < to; i++)
                _xcArraySwapFast(array, i, i + 1);
            return;
        }
        void *valueFrom = _xcArrayGetFast(array, from);
        void *valueTo = _xcArrayGetFast(array, to);
        memcpy(staticBuf, valueFrom, unitSize);
        memmove(valueFrom, valueFrom, (to - from) * unitSize);
        memcpy(valueTo, staticBuf, unitSize);
    } else {
        if (unitSize > STATIC_BUF_SIZE) {
            for (usize i = from; i > to; i--)
                _xcArraySwapFast(array, i, i - 1);
            return;
        }
        void *valueFrom = _xcArrayGetFast(array, from);
        void *valueTo = _xcArrayGetFast(array, to);
        memcpy(staticBuf, valueFrom, unitSize);
        memmove((u8 *)valueTo + unitSize, valueTo, (from - to) * unitSize);
        memcpy(valueTo, staticBuf, unitSize);
    }
}

bool xcArrayMove(XCArray *array, isize from, isize to) {
    from = xcArrayIdxCheck(array, from);
    to = xcArrayIdxCheck(array, to);
    if (from < 0 || to < 0)
        return false;
    _xcArrayMoveFast(array, from, to);
    return true;
}

static void _xcInsertionSort(XCArray *array, XCComparator compareFunc, usize start, usize end) {
    for (usize i = start + 1; i < end; i++) {
        void *movingValue = _xcArrayGetFast(array, i);
        usize finalIndex = i;
        for (usize j = i - 1; j >= start; j--) {
            void *sortedValue = _xcArrayGetFast(array, j);
            if (compareFunc(movingValue, sortedValue) < 0)
                finalIndex = j;
        }
        _xcArrayMoveFast(array, i, finalIndex);
    }
}

static void _xcInPlaceMergeSort(XCArray *array, XCComparator compareFunc, usize start, usize middle, usize end) {
    // Initial sorted items already skipped
    usize i = start;
    usize j = middle;
    usize unitSize = array->unitSize;

    while (i < middle && j < end) {
        void *a = _xcArrayGetFast(array, i);
        void *b = _xcArrayGetFast(array, j);
        if (compareFunc(a, b) < 0) {
            i++;
            continue;
        }
        _xcArrayMoveFast(array, j, i);
        i++;
        j++;
        middle++;
    }
}

static void _xcMergeSort(u8 *tempBuf, XCArray *array, XCComparator compareFunc, usize start, usize middle, usize end) {
    // Skip already sorted initial items
    void *middleValue = _xcArrayGetFast(array, middle);
    while (start < middle) {
        void *startValue = _xcArrayGetFast(array, start);
        if (compareFunc(startValue, middleValue) > 0)
            break;
        start++;
    }
    if (start == middle) // Both sides are already sorted
        return;

    u8 *buf = tempBuf ? tempBuf : malloc((end - start) * array->unitSize);
    if (!buf) {
        _xcInPlaceMergeSort(array, compareFunc, start, middle, end);
        return;
    }

    usize i = start;
    usize j = middle;
    usize k = 0;
    usize unitSize = array->unitSize;

    while (i < middle && j < end) {
        void *a = _xcArrayGetFast(array, i);
        void *b = _xcArrayGetFast(array, j);
        if (compareFunc(a, b) > 0) {
            memcpy(buf + k * unitSize, b, unitSize);
            j++;
        } else {
            memcpy(buf + k * unitSize, a, unitSize);
            i++;
        }
        k++;
    }
    if (i < middle) {
        memcpy(tempBuf + k * unitSize, _xcArrayGetFast(array, i), unitSize * (middle - i));
        k += middle - i;
    }

    memcpy(_xcArrayGetFast(array, start), tempBuf, k * unitSize);
    if (!tempBuf)
        free(buf);
}

void xcArraySort(XCArray *array, XCComparator compareFunc) {
    usize arrLen = array->len;
    for (usize i = 0; i < arrLen; i += SORT_RUN_SIZE)
        _xcInsertionSort(array, compareFunc, i, i + SORT_RUN_SIZE > arrLen ? arrLen : i + SORT_RUN_SIZE);

    // _xcMergeSort deals with failed allocations
    u8 *tempBuf = malloc(arrLen * array->unitSize);
    for (usize size = SORT_RUN_SIZE; size < arrLen; size *= 2) {
        for (usize start = 0; start < arrLen; start += size * 2) {
            usize middle = start + size;
            usize end = start + size * 2;
            if (middle >= arrLen)
                break;
            if (end > arrLen)
                end = arrLen;
            _xcMergeSort(tempBuf, array, compareFunc, start, middle, end);
        }
    }
    if (tempBuf)
        free(tempBuf);
}

// === Item removal ===

bool xcArrayDel(XCArray *array, isize index, XCDestructor itemDestroyFunc) {
    void *value = xcArrayGet(array, index);
    if (!value)
        return false;
    if (itemDestroyFunc)
        itemDestroyFunc(value);
    if (index < 0)
        index += array->len;
    if (index != array->len - 1)
        memmove(value, (u8 *)value + array->unitSize, (array->len - index - 1) * array->unitSize);
    array->len--;
    _xcArrayShrink(array);
    return true;
}

bool xcArrayRemove(XCArray *array, void *value, XCComparator compareFunc, XCDestructor itemDestroyFunc) {
    void *value = xcArrayFind(array, value, compareFunc);
    if (!value)
        return false;
    xcArrayDel(array, xcArrayPtrToIdx(array, value), itemDestroyFunc);
    return true;
}

usize xcArrayRemoveAll(XCArray *array, void *value, XCComparator compareFunc, XCDestructor itemDestroyFunc) {
    usize removedCount = 0;
    u8 *data = (u8 *)array->data;
    for (usize i = 0, n = array->len; i < n; i++) {
        void *remValue = xcArrayGet(array, i);
        if (compareFunc(remValue, value) == 0) {
            removedCount++;
            if (itemDestroyFunc)
                itemDestroyFunc(remValue);
        } else
            memcpy(data + (i - removedCount) * array->unitSize, remValue, array->unitSize);
    }
    array->len -= removedCount;
    _xcArrayShrink(array);
    return removedCount;
}

usize xcArrayRemoveFilter(XCArray *array, XCFilter filterFunc, XCDestructor itemDestroyFunc) {
    usize removedCount = 0;
    u8 *data = (u8 *)array->data;
    for (usize i = 0, n = array->len; i < n; i++) {
        void *remValue = xcArrayGet(array, i);
        if (filterFunc(remValue) == 0) {
            removedCount++;
            if (itemDestroyFunc)
                itemDestroyFunc(remValue);
        } else
            memcpy(data + (i - removedCount) * array->unitSize, remValue, array->unitSize);
    }
    array->len -= removedCount;
    _xcArrayShrink(array);
    return removedCount;
}

// === Iteration ===

void *xcArrayNext(XCArray *array, void *value) {
    if (!value)
        return array->data;
    void *next = ((u8 *)value + array->unitSize);
    if (next > _xcArrayGetFast(array, array->len -  1))
        return NULL;
    return next;
}

// === Index manipulation ===

isize xcArrayIdxCheck(XCArray *array, isize idx) {
    usize len = array->len;
    if (idx < 0)
        idx += len;
    if (idx < 0 || idx >= len)
        return -1;
    return idx;
}

isize xcArrayPtrToIdx(XCArray *array, void *ptr) {
    isize p = (u8 *)ptr - (u8 *)array->data;
    if (p < 0 || p % array->unitSize != 0)
        return -1;
    p /= array->unitSize;
    if (p >= array->len)
        return -1;
    return p;
}

usize xcArrayBisect(XCArray *array, void *value, XCComparator compareFunc) {
    return xcArrayBisectEx(array, value, compareFunc, 0, array->len);
}

usize xcArrayBisectRight(XCArray *array, void *value, XCComparator compareFunc) {
    return xcArrayBisectRightEx(array, value, compareFunc, 0, array->len);
}

isize xcArrayBisectEx(XCArray *array, void *value, XCComparator compareFunc, isize lo, isize hi) {
    lo = xcArrayIdxCheck(array, lo);
    hi = xcArrayIdxCheck(array, hi);
    if (lo < 0 || hi < 0)
        return -1;
    while (lo < hi) {
        isize mid = (lo + hi) / 2;
        void *arrValue = _xcArrayGetFast(array, mid);
        if (compareFunc(value, arrValue) > 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

isize xcArrayBisectRightEx(XCArray *array, void *value, XCComparator compareFunc, isize lo, isize hi) {
    lo = xcArrayIdxCheck(array, lo);
    hi = xcArrayIdxCheck(array, hi);
    if (lo < 0 || hi < 0)
        return -1;
    while (lo < hi) {
        isize mid = (lo + hi) / 2;
        void *arrValue = _xcArrayGetFast(array, mid);
        if (compareFunc(value, arrValue) < 0)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}
