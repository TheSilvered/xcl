#include "../test_lib/test.h"

TestResult test_xcArrayGet_positiveInBounds(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    if (*(int *)xcArrayGet(&array, 0) != 0)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, 1) != 1)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, 2) != 2)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, 3) != 3)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayGet_negativeInBounds(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    if (*(int *)xcArrayGet(&array, -1) != 4)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, -2) != 3)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, -3) != 2)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, -4) != 1)
        return TR_failure;
    else if (*(int *)xcArrayGet(&array, -5) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcArrayGet_outOfBounds(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    // Upper bound
    if (xcArrayGet(&array, 5) != NULL)
        return TR_ignored;
    // Lower bound
    else if (xcArrayGet(&array, -6) != NULL)
        return TR_failure;
    return TR_success;
}

void test_xcArrayGet__addTests(void) {
    testSetModule("test_xcArrayGet");
    testAdd(TEST(test_xcArrayGet_positiveInBounds));
    testAdd(TEST(test_xcArrayGet_negativeInBounds));
    testAdd(TEST(test_xcArrayGet_outOfBounds));
}
