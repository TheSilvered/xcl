#include <string.h>
#include "xc.h"

#define MIN_STR_CAP 16



usize _xcUTF8Size(u32 cp) {
    if (cp <= 0x7f)
        return 1;
    else if (cp <= 0x7ff)
        return 2;
    else if (cp <= 0xffff)
        return 3;
    else
        return 4;
}

usize _xcUTF8Encode(u32 cp, u8 *str) {
    if (cp <= 0x7f) {
        *str = (i8)cp;
        return 1;
    } else if (cp <= 0x7ff) {
        *str++ = 0b11000000 | (i8)(cp >> 6);
        *str   = 0b10000000 | (i8)(cp & 0x3f);
        return 2;
    } else if (cp <= 0xffff) {
        *str++ = 0b11100000 | (i8)(cp >> 12);
        *str++ = 0b10000000 | (i8)(cp >> 6 & 0x3f);
        *str   = 0b10000000 | (i8)(cp & 0x3f);
        return 3;
    } else {
        *str++ = 0b11110000 | (i8)(cp >> 18);
        *str++ = 0b10000000 | (i8)(cp >> 12 & 0x3f);
        *str++ = 0b10000000 | (i8)(cp >> 6 & 0x3f);
        *str   = 0b10000000 | (i8)(cp & 0x3f);
        return 4;
    }
}

// === Creation & Initialization ===

XCLIB bool xcStrInit(XCStr *str, const char *value) {
    usize size = strlen(value);
    str->data = malloc(size * sizeof(u8));
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

XCLIB void xcStrInitFromData(XCStr *str, u8 *data, usize size) {
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

XCLIB XCStr *xcStrNewFromData(u8 *data, usize size) {
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

// === Destruction ===

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

// === Hashing ===

XCLIB u32 xcStrHash(XCRef str) {
    // Fowler–Noll–Vo 32-bit hash function

    u8 *data = ((XCStr *)str)->data;
    usize size = ((XCStr *)str)->size;

    u32 hash = 0x811c9dc5;

    for (usize i = 0; i < size; i++) {
        hash ^= (u32)data[i];
        hash *= 0x01000193;
    }

    return hash;
}

// === Comparison ===

XCLIB int xcStrCompare(XCRef str1, XCRef str2) {
    u8 *p1 = ((XCStr *)str1)->data;
    u8 *p2 = ((XCStr *)str2)->data;
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

// === Addition ===

static inline bool _xcStrExpand(XCStr *str, usize quantity) {
    if (str->size + quantity <= str->cap)
        return true;
    usize cap = str->size + quantity;
    usize newCap = cap + (cap >> 1); // Expand capacity by 1.5
    XCMemBlock newData = realloc(str->data, newCap);
    if (!newData)
        return false;
    str->data = newData;
    str->cap = newCap;
    return true;
}

XCLIB bool xcStrAppendUnicode(XCStr *str, u32 *codes, usize codeCount) {
    usize sizeNeeded = 0;
    for (usize i = 0; i < codeCount; i++) {
        sizeNeeded += _xcUTF8Size(codes[i]);
    }
    if (!_xcStrExpand(str, sizeNeeded))
        return false;
    for (usize i = 0; i < codeCount; i++) {
        str->size += _xcUTF8Encode(codes[i], str->data + str->size);
    }
    return true;
}
