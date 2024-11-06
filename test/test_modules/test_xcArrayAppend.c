#include "test.h"
#include "xc.h"

TestResult test_xcArrayAppend_fromEmpty(void) {
    XCArray *array = xcArrayNew(sizeof(int), 0);
    TestResult result = TR_success;
    if (!array)
        return TR_failure;
    int val = 1;
    if (!xcArrayAppend(array, &val))
        result = TR_allocFailed;
    else if (array->len != 1)
        result = TR_failure;

    xcArrayFree(array, NULL);
    return result;
}

TestResult test_xcArrayAppend_forceExpansion(void) {
    XCArray *array = xcArrayNew(sizeof(int), 0);
    TestResult result = TR_success;
    if (!array)
        return TR_failure;

    for (int i = 0; i < 100; i++) {
        if (!xcArrayAppend(array, &i)) {
            result = TR_allocFailed;
            break;
        } else if (array->len != i + 1) {
            result = TR_failure;
            break;
        }
    }

    xcArrayFree(array, NULL);
    return result;
}

void test_xcArrayAppend__addTests(void) {
    testSetModule("test_xcArrayAppend");
    testAdd(TEST(test_xcArrayAppend_fromEmpty));
    testAdd(TEST(test_xcArrayAppend_forceExpansion));
}
