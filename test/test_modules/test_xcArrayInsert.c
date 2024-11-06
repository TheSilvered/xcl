#include "test.h"
#include "xc.h"

TestResult test_xcArrayInsert_middle(void) {
    int data[] = { 0, 1, 2, 4, 5 };
    XCArray *array = xcArrayNewFromCopy(sizeof(int), sizeof(data) / sizeof(int), data);
    if (!array)
        return TR_allocFailed;

    TestResult result = TR_success;
    int value = 3;
    if (!xcArrayInsert(array, &value, 3))
        result = TR_allocFailed;
    else {
        for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
            if (*(int *)xcArrayGet(array, i) != i) {
                result = TR_failure;
                break;
            }
        }
    }
    xcArrayFree(array, NULL);
    return result;
}

TestResult test_xcArrayInsert_start(void) {
    int data[] = { 1, 2, 3, 4, 5 };
    XCArray *array = xcArrayNewFromCopy(sizeof(int), sizeof(data) / sizeof(int), data);
    if (!array)
        return TR_allocFailed;

    TestResult result = TR_success;
    int value = 0;
    if (!xcArrayInsert(array, &value, 0))
        result = TR_allocFailed;
    else {
        for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
            if (*(int *)xcArrayGet(array, i) != i) {
                result = TR_failure;
                break;
            }
        }
    }
    xcArrayFree(array, NULL);
    return result;
}

TestResult test_xcArrayInsert_end(void) {
    int data[] = { 0, 1, 2, 3, 5 };
    XCArray *array = xcArrayNewFromCopy(sizeof(int), sizeof(data) / sizeof(int), data);
    if (!array)
        return TR_allocFailed;

    TestResult result = TR_success;
    int value = 4;
    if (!xcArrayInsert(array, &value, 4))
        result = TR_allocFailed;
    else {
        for (int i = 0; i < sizeof(data) / sizeof(int); i++) {
            if (*(int *)xcArrayGet(array, i) != i) {
                result = TR_failure;
                break;
            }
        }
    }
    xcArrayFree(array, NULL);
    return result;
}

void test_xcArrayInsert__addTests(void) {
    testSetModule("test_xcArrayInsert");
    testAdd(TEST(test_xcArrayInsert_middle));
    testAdd(TEST(test_xcArrayInsert_start));
    testAdd(TEST(test_xcArrayInsert_end));
}
