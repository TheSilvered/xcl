#include "../test_lib/test.h"

TestResult test_xcArrayFindAll_countOneMatch(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 3;
    void *value = xcArrayFind(&array, &valueToFind, xcCompare_int);
    if (xcArrayPtrToIdx(&array, value) != 3)
        return TR_failure;
    if (*(int *)value != 3)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFindAll_oneMatch(void) {

}

TestResult test_xcArrayFindAll_countZeroMatches(void) {

}

TestResult test_xcArrayFindAll_zeroMatches(void) {

}

TestResult test_xcArrayFindAll_countManyMatches(void) {

}

TestResult test_xcArrayFindAll_manyMatches(void) {

}

void test_xcArrayFindAll__addTests(void) {
    testSetModule("test_xcArrayFindAll");
}
