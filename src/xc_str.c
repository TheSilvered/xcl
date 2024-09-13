#include <string.h>
#include "xc.h"

#define MIN_STR_CAP 16

bool xcStrInit(XCStr *str, const char *value) {
    usize len = strlen(value);
    str->data = malloc(len * sizeof(char));
    if (!str->data)
        return false;
    memcpy(str->data, value, len);
    str->len = len;
    str->cap = len;
    return true;
}

bool xcStrInitEmpty(XCStr *str) {
    str->data = malloc(MIN_STR_CAP * sizeof(char));
    if (!str->data)
        return false;
    str->len = 0;
    str->cap = MIN_STR_CAP;
    return true;
}

void xcStrInitFromData(XCStr *str, char *data, usize len) {
    str->data = data;
    str->len = len;
    str->cap = len;
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

XCStr *xcStrNewFromData(char *data, usize len) {
    XCStr *str = malloc(sizeof(XCStr));
    if (!str)
        return NULL;
    xcStrInitFromData(str, data, len);
    return str;
}

void xcStrViewFromXCStr(XCStrView *strView, XCStr *str) {
    strView->data = str->data;
    strView->len = str->len;
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
