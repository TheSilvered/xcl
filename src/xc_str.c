#include <string.h>
#include "xc.h"

#define MIN_STR_CAP 16

bool xcStrInit(XCStr *str, const char *value) {
    usize size = strlen(value);
    str->data = malloc(size * sizeof(char));
    if (!str->data)
        return false;
    memcpy(str->data, value, size);
    str->size = size;
    str->cap = size;
    return true;
}

bool xcStrInitEmpty(XCStr *str) {
    str->data = malloc(MIN_STR_CAP * sizeof(char));
    if (!str->data)
        return false;
    str->size = 0;
    str->cap = MIN_STR_CAP;
    return true;
}

void xcStrInitFromData(XCStr *str, char *data, usize size) {
    str->data = data;
    str->size = size;
    str->cap = size;
}

XCStr *xcStrNew(const char *value) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    if (!xcStrInit(str, value)) {
        free(str);
        return NULL;
    }
    return str;
}

XCStr *xcStrNewEmpty(void) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    if (!xcStrInitEmpty(str)) {
        free(str);
        return NULL;
    }
    return str;
}

XCStr *xcStrNewFromData(char *data, usize size) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    xcStrInitFromData(str, data, size);
    return str;
}

void xcStrViewInitFromXCStr(XCStrView *strView, XCStr *str) {
    strView->data = str->data;
    strView->size = str->size;
}

void xcStrDestroy(XCStr *str) {
    if (!str)
        return;
    free(str->data);
}

void xcStrFree(XCStr *str) {
    if (!str)
        return;
    xcStrDestroy(str);
    free(str);
}
