#include "../test_lib/test.h"

TestResult test_xcArrayInit_empty(void) {
    XCArray array;
    bool result = xcArrayInit(&array, sizeof(int), 0);
    if (!result)
        return TR_allocFailed;
    if (array.len != 0)
        return TR_failure;
    if (array.cap != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayInit_reserve(void) {
    XCArray array;
    const usize reserve = 20;
    bool result = xcArrayInit(&array, sizeof(int), reserve);
    if (!result)
        return TR_allocFailed;
    if (array.len != 0)
        return TR_failure;
    if (array.cap != reserve)
        return TR_failure;
    if (xcDebug_BlockSize(array.data) != reserve * sizeof(int))
        return TR_failure;
    return TR_success;
}

void test_xcArrayInit__addTests(void) {
    testSetModule("test_xcArrayInit");
    testAdd(TEST(test_xcArrayInit_empty));
    testAdd(TEST(test_xcArrayInit_reserve));
}
