#include <string.h>
#include "xc.h"

XCBoolArray xcBoolArrayNew(usize length) {
    return calloc(xcBoolArraySize(length), sizeof(XCBoolArrayChunk));
}

void xcBoolArrayFree(XCBoolArray array) {
    free(array);
}

bool xcBoolArrayGet(XCBoolArray array, usize idx) {
    return (bool)(array[idx / 8] & (1 << (idx % 8)));
}

void xcBoolArraySet(XCBoolArray array, usize idx, bool value) {
    u8 byte = array[idx / 8];
    if (value)
        array[idx / 8] = byte | (1 << (idx % 8));
    else
        array[idx / 8] = byte & ~(1 << (idx % 8));
}

void xcBoolArraySetAll(XCBoolArray array, usize length, bool value) {
    memset(array, value ? 255 : 0, xcBoolArraySize(length));
}
