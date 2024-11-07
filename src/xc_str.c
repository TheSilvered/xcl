#include <string.h>
#include "xc.h"

#define MIN_STR_CAP 16

XCLIB bool xcStrInit(XCStr *str, const char *value) {
    usize size = strlen(value);
    str->data = malloc(size * sizeof(char));
    if (!str->data)
        return false;
    memcpy(str->data, value, size);
    str->size = size;
    str->cap = size;
    return true;
}

XCLIB bool xcStrInitEmpty(XCStr *str) {
    str->data = malloc(MIN_STR_CAP * sizeof(char));
    if (!str->data)
        return false;
    str->size = 0;
    str->cap = MIN_STR_CAP;
    return true;
}

XCLIB void xcStrInitFromData(XCStr *str, char *data, usize size) {
    str->data = data;
    str->size = size;
    str->cap = size;
}

XCLIB XCStr *xcStrNew(const char *value) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    if (!xcStrInit(str, value)) {
        free(str);
        return NULL;
    }
    return str;
}

XCLIB XCStr *xcStrNewEmpty(void) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    if (!xcStrInitEmpty(str)) {
        free(str);
        return NULL;
    }
    return str;
}

XCLIB XCStr *xcStrNewFromData(char *data, usize size) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    xcStrInitFromData(str, data, size);
    return str;
}

XCLIB void xcStrViewInitFromXCStr(XCStrView *strView, XCStr *str) {
    strView->data = str->data;
    strView->size = str->size;
}

XCLIB void xcStrDestroy(XCStr *str) {
    if (!str)
        return;
    free(str->data);
}

XCLIB void xcStrFree(XCStr *str) {
    if (!str)
        return;
    xcStrDestroy(str);
    free(str);
}

XCLIB u32 xcStrHash(XCRef str) {
    // Fowler–Noll–Vo 32-bit hash function

    char *data = ((XCStr *)str)->data;
    usize size = ((XCStr *)str)->size;

    u32 hash = 0x811c9dc5;

    for (usize i = 0; i < size; i++) {
        hash ^= (u32)data[i];
        hash *= 0x01000193;
    }

    return hash;
}

XCLIB int xcStrCompare(XCRef str1, XCRef str2) {
    char *p1 = ((XCStr *)str1)->data;
    char *p2 = ((XCStr *)str2)->data;
    usize size1 = ((XCStr *)str1)->size;
    usize size2 = ((XCStr *)str2)->size;

    usize i = 0;

    for (usize i = 0; i < size1 && i < size2; i++) {
        if (p1[i] != p2[i])
            return (u8)p1[i] < (u8)p2[i] ? -1 : 1;
    }

    if (size1 == size2)
        return 0;
    return size1 < size2 ? -1 : 1;
}
