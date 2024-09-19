#include "../test_lib/test.h"

TestResult test_xcArrayInitFromData_main(void) {
    XCArray array;
    const usize arrLength = 5;
    int *cArr = malloc(arrLength * sizeof(int));
    if (!cArr) return TR_allocFailed;
    for (int i = 0; i < arrLength; i++) cArr[i] = i + 1;

    TestResult result = TR_success;
    xcArrayInitFromData(&array, sizeof(int), arrLength, cArr);
    if (array.len != arrLength || array.cap != arrLength)
        result = TR_failure;
    else if (array.data != cArr)
        result = TR_failure;
    xcArrayDestroy(&array, NULL);
    return result;
}

void test_xcArrayInitFromData__addTests(void) {
    testSetModule("test_xcArrayInitFromData");
    testAdd(TEST(test_xcArrayInitFromData_main));
}
