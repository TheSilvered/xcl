#include "../test_lib/test.h"
#include <string.h>

TestResult test_xcArraySwap_sameIndex(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArraySwap(&array, 0, 0))
        return TR_failure;
    if (!xcArraySwap(&array, 1, 1))
        return TR_failure;
    if (!xcArraySwap(&array, 2, 2))
        return TR_failure;
    if (!xcArraySwap(&array, 3, 3))
        return TR_failure;
    if (!xcArraySwap(&array, 4, 4))
        return TR_failure;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        if (*(int *)xcArrayGet(&array, i) != i)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArraySwap_sameIndexNegative(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArraySwap(&array, -1, -1))
        return TR_failure;
    if (!xcArraySwap(&array, -2, -2))
        return TR_failure;
    if (!xcArraySwap(&array, -3, -3))
        return TR_failure;
    if (!xcArraySwap(&array, -4, -4))
        return TR_failure;
    if (!xcArraySwap(&array, -5, -5))
        return TR_failure;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        if (*(int *)xcArrayGet(&array, i) != i)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArraySwap_differentIndex(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArraySwap(&array, 1, 4))
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 1) != 4)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 4) != 1)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArraySwap_differentIndexNegative(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArraySwap(&array, -2, -3))
        return TR_failure;
    if (*(int *)xcArrayGet(&array, -2) != 2)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, -3) != 3)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArraySwap_bigItem(void) {
    XCArray array;
    u8 data[3][256];
    memset(data[0], 15, 256);
    memset(data[1], 60, 256);
    memset(data[2], 240, 256);
    xcArrayInitFromData(&array, 256, 3, data);
    if (!xcArraySwap(&array, 0, 2))
        return TR_failure;
    for (int i = 0; i < 256; i++) {
        if (data[0][i] != 240)
            return TR_failure;
    }
    for (int i = 0; i < 256; i++) {
        if (data[1][i] != 60)
            return TR_failure;
    }
    for (int i = 0; i < 256; i++) {
        if (data[2][i] != 15)
            return TR_failure;
    }
    return TR_success;
}

void test_xcArraySwap__addTests(void) {
    testSetModule("test_xcArraySwap");
    testAdd(TEST(test_xcArraySwap_sameIndex));
    testAdd(TEST(test_xcArraySwap_sameIndexNegative));
    testAdd(TEST(test_xcArraySwap_differentIndexNegative));
    testAdd(TEST(test_xcArraySwap_bigItem));
}
