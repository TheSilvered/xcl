#include "xc.h"
#include <string.h>
#include <stdio.h>

#define MIN_ARRAY_SIZE 8
#define SORT_RUN_SIZE 32
#define STATIC_BUF_SIZE 64

XCArray *xcArrayNew(usize unitSize, usize reserve) {
    XCArray *array = (XCArray *)malloc(sizeof(XCArray));
    if (!array)
        return NULL;

    array->data = malloc((MIN_ARRAY_SIZE > reserve ? MIN_ARRAY_SIZE : reserve) * unitSize);
    if (!array->data) {
        free(array);
        return NULL;
    }
    array->cap = reserve;
    array->len = 0;
    array->unitSize = unitSize;
    return array;
}

static XCBool _xcArrayExpand(XCArray *array) {
    if (array->len < array->cap)
        return True;
    usize newCap = (usize)((double)array->cap * 1.5);
    void *newData = realloc(array->data, newCap * array->unitSize);
    if (!newData)
        return False;
    array->data = newData;
    array->cap = newCap;
    return True;
}

static void _xcArrayShrink(XCArray *array) {
    if (array->len >= array->cap / 4)
        return;
    usize newCap = array->cap / 2;
    if (newCap < MIN_ARRAY_SIZE)
        return;
    void *newData = realloc(array->data, newCap * array->unitSize);
    if (!newData)
        return;
    array->data = newData;
    array->cap = newCap;
}

XCBool xcArrayAppend(XCArray *array, void *value) {
    if (!_xcArrayExpand(array))
        return False;
    memcpy((u8 *)array->data + array->len * array->unitSize, value, array->unitSize);
    array->len++;
    return True;
}

void *_xcArrayGetFast(XCArray *array, usize index) {
    return (void *)((u8 *)array->data + index * array->unitSize);
}

void *xcArrayGet(XCArray *array, isize index) {
    if (index < 0)
        index += array->len;
    if (index < 0 || index >= array->len)
        return NULL;
    return _xcArrayGetFast(array, (usize)index);
}

void _xcArraySetFast(XCArray *array, void *value, usize index) {
    memcpy(_xcArrayGetFast(array, index), value, array->unitSize);
}

XCBool xcArraySet(XCArray *array, void *value, isize index, XCDestructor itemDestroyFunc) {
    void *arrayValue = xcArrayGet(array, index);
    if (!arrayValue)
        return False;
    if (itemDestroyFunc)
        itemDestroyFunc(arrayValue);
    memcpy(arrayValue, value, array->unitSize);
}

XCBool xcArrayRemove(XCArray *array, isize index, XCDestructor itemDestroyFunc) {
    void *value = xcArrayGet(array, index);
    if (!value)
        return False;
    if (itemDestroyFunc)
        itemDestroyFunc(value);
    if (index < 0)
        index += array->len;
    if (index != array->len - 1)
        memmove(value, (u8 *)value + array->unitSize, (array->len - index - 1) * array->unitSize);
    array->len--;
    _xcArrayShrink(array);
    return True;
}

void _xcArraySwapFast(XCArray *array, usize index1, usize index2) {
    u8 *v1 = (u8 *)_xcArrayGetFast(array, index1);
    u8 *v2 = (u8 *)_xcArrayGetFast(array, index2);
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

XCBool xcArraySwap(XCArray *array, isize index1, isize index2) {
    if (index1 < 0)
        index1 += array->len;
    if (index2 < 0)
        index2 += array->len;

    if (index1 < 0 || index2 >= array->len || index2 < 0 || index2 >= array->len)
        return False;

    _xcArraySwapFast(array, index1, index2);
    return True;
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

XCBool xcArrayMove(XCArray *array, isize from, isize to) {
    if (from < 0)
        from += array->len;
    if (to < 0)
        to += array->len;
    if (from < 0 || from >= array->len || to < 0 || to >= array->len)
        return False;

    _xcArrayMoveFast(array, from, to);
    return True;
}

static void _xcInsertionSort(XCArray *array, XCComparator compareFunc, usize start, usize end) {
    for (usize i = start + 1; i < end; i++) {
        void *movingValue = _xcArrayGetFast(array, i);
        usize finalIndex = i;
        for (usize j = i - 1; j >= start; j--) {
            void *sortedValue = _xcArrayGetFast(array, j);
            if (compareFunc(movingValue, sortedValue) == XC_CR_LESS_THAN)
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
        if (compareFunc(a, b) == XC_CR_LESS_THAN) {
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
        if (compareFunc(startValue, middleValue) == XC_CR_GREATER_THAN)
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
        if (compareFunc(a, b) == XC_CR_GREATER_THAN) {
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
    u8 *tempBuf = (u8 *)malloc(arrLen * array->unitSize);
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

void *xcArrayNext(XCArray *array, void *value) {
    if (!value)
        return array->data;
    void *next = (void *)((u8 *)value + array->unitSize);
    if (next > _xcArrayGetFast(array, array->len -  1))
        return NULL;
    return next;
}

usize xcArrayCount(XCArray *array, void *value, XCComparator compareFunc) {
    usize count = 0;
    for (void *v = xcArrayNext(array, NULL); v; v = xcArrayNext(array, v)) {
        if (compareFunc(v, value) == XC_CR_EQUAL)
            count++;
    }
    return count;
}

usize xcArrayRemoveAll(XCArray *array, void *value, XCComparator compareFunc, XCDestructor itemDestroyFunc) {
    usize removedCount = 0;
    u8 *data = (u8 *)array->data;
    for (usize i = 0, n = array->len; i < n; i++) {
        void *remValue = xcArrayGet(array, i);
        if (compareFunc(remValue, value) == XC_CR_EQUAL) {
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

void xcArrayDestroy(XCArray *array, XCDestructor itemDestroyFunc) {
    if (itemDestroyFunc) {
        for (void *v = xcArrayNext(array, NULL); v; v = xcArrayNext(array, v))
            itemDestroyFunc(v);
    }
    free(array->data);
    free(array);
}
