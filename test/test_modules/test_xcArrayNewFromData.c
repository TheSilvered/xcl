#include "test.h"
#include "xc.h"

TestResult test_xcArrayNewFromData_main(void) {
    const usize arrLength = 5;
    int *cArr = malloc(arrLength * sizeof(int));
    if (!cArr) return TR_allocFailed;
    for (int i = 0; i < arrLength; i++) cArr[i] = i + 1;

    TestResult result = TR_success;
    XCArray *array = xcArrayNewFromData(sizeof(int), arrLength, cArr);
    if (!array) {
        free(cArr);
        return TR_allocFailed;
    }
    if (array->len != arrLength || array->cap != arrLength)
        result = TR_failure;
    else if (array->data != cArr)
        result = TR_failure;
    xcArrayFree(array, NULL);
    return result;
}

void test_xcArrayNewFromData__addTests(void) {
    testSetModule("test_xcArrayNewFromData");
    testAdd(TEST(test_xcArrayNewFromData_main));
}
