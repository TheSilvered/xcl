#include <stdio.h>
#include <string.h>
#include "xc.h"

static char getStr_str[256] = { 0 };

const char *getStr(const char *msg) {
    printf("%s", msg);
    scanf("%255s", getStr_str);
    return (const char *)getStr_str;
}

void setKey(XCMap *map) {
    XCStr key, value;
    const char *keyStr = getStr("Key> ");
    xcStrInit(&key, keyStr);
    const char *valueStr = getStr("Value> ");
    xcStrInit(&value, valueStr);
    xcMapSet(map, &key, &value, (XCDestructor)xcStrDestroy, (XCDestructor)xcStrDestroy);
}

void getKey(XCMap *map) {
    XCStr key;
    const char *keyStr = getStr("Key> ");
    xcStrInit(&key, keyStr);
    XCStr *value = xcMapGet(map, &key);
    xcStrDestroy(&key);

    if (!value)
        printf("The key is not in the map.\n");
    else
        printf("Value: %.*s\n", (int)value->size, value->data);
}

void delKey(XCMap *map) {
    XCStr key;
    const char *keyStr = getStr("Key> ");
    xcStrInit(&key, keyStr);
    if (xcMapDel(map, &key, (XCDestructor)xcStrDestroy, (XCDestructor)xcStrDestroy))
        printf("Key removed.\n");
    else
        printf("The key is not in the map.\n");
}

void listKeys(XCMap *map) {
    XCStr *value;
    for (XCStr *key = xcMapNext(map, NULL, (XCRef *)&value); key != NULL; key = xcMapNext(map, key, (XCRef *)&value)) {
        printf("%.*s: %.*s\n", (int)key->size, key->data, (int)value->size, value->data);
    }
}

int main(void) {
    XCMap map;
    if (!xcMapInit(&map, xcStrHash, xcStrCompare, sizeof(XCStr), sizeof(XCStr)))
        return 1;

    while (true) {
        const char *msg = getStr("> ");
        if (strcmp(msg, "get") == 0)
            getKey(&map);
        else if (strcmp(msg, "set") == 0)
            setKey(&map);
        else if (strcmp(msg, "del") == 0)
            delKey(&map);
        else if (strcmp(msg, "list") == 0)
            listKeys(&map);
        else if (strcmp(msg, "cap") == 0)
            printf("map.cap = %zi\n", map.cap);
        else if (strcmp(msg, "len") == 0)
            printf("map.len = %zi\n", map.len);
        else if (strcmp(msg, "exit") == 0)
            break;
        else
            printf("Invalid command. Valid commands are 'get', 'set', 'del', 'list' and 'exit'.\n");
    }

    xcMapDestroy(&map, (XCDestructor)xcStrDestroy, (XCDestructor)xcStrDestroy);
    return 0;
}
