#include <string.h>
#include "xc.h"

XCLIB XCBoolArray xcBoolArrayNew(usize length) {
    return calloc(xcBoolArraySize(length), sizeof(XCBoolArrayChunk));
}

XCLIB void xcBoolArrayFree(XCBoolArray array) {
    free(array);
}

XCLIB bool xcBoolArrayGet(XCBoolArray array, usize idx) {
    return (bool)(array[idx >> 3] & (1 << (idx & 0b111)));
}

XCLIB void xcBoolArraySet(XCBoolArray array, usize idx, bool value) {
    u8 byte = array[idx >> 3];
    if (value)
        array[idx >> 3] = byte | (1 << (idx & 0b111));
    else
        array[idx >> 3] = byte & ~(1 << (idx & 0b111));
}

XCLIB void xcBoolArraySetAll(XCBoolArray array, usize length, bool value) {
    memset(array, value ? 255 : 0, xcBoolArraySize(length));
}
