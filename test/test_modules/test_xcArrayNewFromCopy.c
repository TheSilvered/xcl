#include "../test_lib/test.h"

TestResult test_xcArrayNewFromCopy_empty(void) {
    int cArr[0] = { };
    XCArray *array = xcArrayNewFromCopy(sizeof(int), 0, cArr);
    TestResult result = TR_success;
    if (!array)
        return TR_allocFailed;
    if (array->len != 0 || array->cap != 0)
        result = TR_failure;

    xcArrayFree(array, NULL);
    return result;
}

TestResult test_xcArrayNewFromCopy_full(void) {
    int cArr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TestResult result = TR_success;
    const usize arrLength = sizeof(cArr) / sizeof(int);
    XCArray *array = xcArrayNewFromCopy(sizeof(int), arrLength, cArr);
    if (!array)
        return TR_allocFailed;
    if (array->len != arrLength || array->cap != arrLength)
        result = TR_failure;
    else if (xcDebug_BlockSize(array->data) != arrLength * sizeof(int))
        result = TR_failure;
    else {
        for (int i = 0; i < arrLength; i++) {
            if (*(int *)xcArrayGet(array, i) != i + 1) {
                result = TR_failure;
                break;
            }
        }
    }
    xcArrayFree(array, NULL);
    return result;
}

void test_xcArrayNewFromCopy__addTests(void) {
    testSetModule("test_xcArrayNewFromCopy");
    testAdd(TEST(test_xcArrayNewFromCopy_empty));
    testAdd(TEST(test_xcArrayNewFromCopy_full));
}
