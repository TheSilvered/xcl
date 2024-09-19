#include "test.h"

TestResult test_xcArrayInit_empty(void) {
    XCArray array;
    TestResult result = TR_success;
    if (!xcArrayInit(&array, sizeof(int), 0))
        return TR_allocFailed;
    if (array.len != 0 || array.cap != 0)
        result = TR_failure;
    xcArrayDestroy(&array, NULL);
    return result;
}

TestResult test_xcArrayInit_reserve(void) {
    XCArray array;
    const usize reserve = 20;
    TestResult result = TR_success;
    if (!xcArrayInit(&array, sizeof(int), reserve))
        return TR_allocFailed;
    if (array.len != 0)
        result = TR_failure;
    else if (array.cap != reserve)
        result = TR_failure;
    else if (xcDebug_BlockSize(array.data) != reserve * sizeof(int))
        result = TR_failure;
    xcArrayDestroy(&array, NULL);
    return result;
}

void test_xcArrayInit__addTests(void) {
    testSetModule("test_xcArrayInit");
    testAdd(TEST(test_xcArrayInit_empty));
    testAdd(TEST(test_xcArrayInit_reserve));
}
