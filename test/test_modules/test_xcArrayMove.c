#include "test.h"
#include "xc.h"

TestResult test_xcArrayMove_sameIndex(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArrayMove(&array, 0, 0))
        return TR_failure;
    if (!xcArrayMove(&array, 1, 1))
        return TR_failure;
    if (!xcArrayMove(&array, 2, 2))
        return TR_failure;
    if (!xcArrayMove(&array, 3, 3))
        return TR_failure;
    if (!xcArrayMove(&array, 4, 4))
        return TR_failure;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        if (*(int *)xcArrayGet(&array, i) != i)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArrayMove_sameIndexNegative(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArrayMove(&array, -1, -1))
        return TR_failure;
    if (!xcArrayMove(&array, -2, -2))
        return TR_failure;
    if (!xcArrayMove(&array, -3, -3))
        return TR_failure;
    if (!xcArrayMove(&array, -4, -4))
        return TR_failure;
    if (!xcArrayMove(&array, -5, -5))
        return TR_failure;
    for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
        if (*(int *)xcArrayGet(&array, i) != i)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArrayMove_differentIndexForward(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArrayMove(&array, 1, 4))
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 0) != 0)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 1) != 2)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 2) != 3)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 3) != 4)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 4) != 1)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayMove_differentIndexBackward(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArrayMove(&array, 4, 1))
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 0) != 0)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 1) != 4)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 2) != 1)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 3) != 2)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 4) != 3)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayMove_differentIndexNegativeForward(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArrayMove(&array, -4, -2))
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 0) != 0)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 1) != 2)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 2) != 3)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 3) != 1)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayMove_differentIndexNegativeBackward(void) {
    XCArray array;
    int data[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(data) / sizeof(int), data);
    if (!xcArrayMove(&array, -2, -4))
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 0) != 0)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 1) != 3)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 2) != 1)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 3) != 2)
        return TR_failure;
    if (*(int *)xcArrayGet(&array, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayMove_bigItemForward(void) {
    XCArray array;
    u8 data[3][256];
    memset(data[0], 15, 256);
    memset(data[1], 60, 256);
    memset(data[2], 240, 256);
    xcArrayInitFromData(&array, 256, 3, data);
    if (!xcArrayMove(&array, 0, 2))
        return TR_failure;
    for (int i = 0; i < 256; i++) {
        if (data[0][i] != 60)
            return TR_failure;
    }
    for (int i = 0; i < 256; i++) {
        if (data[1][i] != 240)
            return TR_failure;
    }
    for (int i = 0; i < 256; i++) {
        if (data[2][i] != 15)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArrayMove_bigItemBackward(void) {
    XCArray array;
    u8 data[3][256];
    memset(data[0], 15, 256);
    memset(data[1], 60, 256);
    memset(data[2], 240, 256);
    xcArrayInitFromData(&array, 256, 3, data);
    if (!xcArrayMove(&array, 2, 0))
        return TR_failure;
    for (int i = 0; i < 256; i++) {
        if (data[0][i] != 240)
            return TR_failure;
    }
    for (int i = 0; i < 256; i++) {
        if (data[1][i] != 15)
            return TR_failure;
    }
    for (int i = 0; i < 256; i++) {
        if (data[2][i] != 60)
            return TR_failure;
    }
    return TR_success;
}

void test_xcArrayMove__addTests(void) {
    testSetModule("test_xcArrayMove");
    testAdd(TEST(test_xcArrayMove_sameIndex));
    testAdd(TEST(test_xcArrayMove_sameIndexNegative));
    testAdd(TEST(test_xcArrayMove_differentIndexForward));
    testAdd(TEST(test_xcArrayMove_differentIndexBackward));
    testAdd(TEST(test_xcArrayMove_differentIndexNegativeForward));
    testAdd(TEST(test_xcArrayMove_differentIndexNegativeBackward));
    testAdd(TEST(test_xcArrayMove_bigItemForward));
    testAdd(TEST(test_xcArrayMove_bigItemBackward));
}
