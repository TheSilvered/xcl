#include "../test_lib/test.h"

TestResult test_xcArrayNew_empty(void) {
    XCArray *array = xcArrayNew(sizeof(int), 0);
    TestResult result = TR_success;
    if (!array)
        return TR_allocFailed;
    if (array->len != 0 || array->cap != 0)
        result = TR_failure;
    xcArrayFree(array, NULL);
    return result;
}

TestResult test_xcArrayNew_reserve(void) {
    const usize reserve = 20;
    XCArray *array = xcArrayNew(sizeof(int), reserve);
    TestResult result = TR_success;
    if (!array)
        return TR_allocFailed;
    if (array->len != 0)
        result = TR_failure;
    else if (array->cap != reserve)
        result = TR_failure;
    else if (xcDebug_BlockSize(array->data) != reserve * sizeof(int))
        result = TR_failure;
    xcArrayFree(array, NULL);
    return result;
}

void test_xcArrayNew__addTests(void) {
    testSetModule("test_xcArrayNew");
    testAdd(TEST(test_xcArrayNew_empty));
    testAdd(TEST(test_xcArrayNew_reserve));
}
