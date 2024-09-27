#include "../test_lib/test.h"

TestResult test_xcArrayFind_oneMatch(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 3;
    XCRef value = xcArrayFind(&array, &valueToFind, xcCompare_int);
    if (xcArrayRefToIdx(&array, value) != 3)
        return TR_failure;
    if (*(int *)value != 3)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFind_zeroMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 5;
    XCRef value = xcArrayFind(&array, &valueToFind, xcCompare_int);
    if (value != NULL)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayFind_manyMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 3, 2, 3, 3 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 3;
    XCRef value = xcArrayFind(&array, &valueToFind, xcCompare_int);
    if (xcArrayRefToIdx(&array, value) != 1)
        return TR_failure;
    if (*(int *)value != 3)
        return TR_failure;

    return TR_success;
}

void test_xcArrayFind__addTests(void) {
    testSetModule("test_xcArrayFind");
    testAdd(TEST(test_xcArrayFind_oneMatch));
    testAdd(TEST(test_xcArrayFind_zeroMatches));
    testAdd(TEST(test_xcArrayFind_manyMatches));
}
