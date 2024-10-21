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
