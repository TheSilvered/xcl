#include "../test_lib/test.h"

TestResult test_xcArrayExtend_empty(void) {
    int newData[] = { 1, 2, 3 };
    XCArray *array = xcArrayNew(sizeof(int), 0);
    if (!array)
        return TR_allocFailed;
    TestResult result = TR_success;
    if (!xcArrayExtend(array, sizeof(newData) / sizeof(int), newData))
        result = TR_allocFailed;
    else if (*(int *)xcArrayGet(array, 0) != 1)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 1) != 2)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 2) != 3)
        result = TR_failure;
    xcArrayFree(array, NULL);
    return result;
}

TestResult test_xcArrayExtend_full(void) {
    int newData[] = { 1, 2, 3 };
    int arrData[] = { 3, 4, 5 };
    XCArray *array = xcArrayNewFromCopy(sizeof(int), sizeof(arrData) / sizeof(int), arrData);
    if (!array)
        return TR_allocFailed;
    TestResult result = TR_success;
    if (!xcArrayExtend(array, sizeof(newData) / sizeof(int), newData))
        result = TR_allocFailed;
    else if (*(int *)xcArrayGet(array, 0) != 3)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 1) != 4)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 2) != 5)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 3) != 1)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 4) != 2)
        result = TR_failure;
    else if (*(int *)xcArrayGet(array, 5) != 3)
        result = TR_failure;
    xcArrayFree(array, NULL);
    return result;
}

void test_xcArrayExtend__addTests(void) {
    testSetModule("test_xcArrayExtend");
    testAdd(TEST(test_xcArrayExtend_empty));
    testAdd(TEST(test_xcArrayExtend_full));
}
