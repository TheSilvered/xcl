#include "test.h"
#include "xc.h"

TestResult test_xcArrayFindAll_countOneMatch(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 3;
    usize count = xcArrayFindAll(&array, &valueToFind, xcCompare_int, NULL);
    if (count != 1)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFindAll_oneMatch(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);
    TestResult result = TR_success;

    int valueToFind = 3;
    XCRef *refs;
    usize count = xcArrayFindAll(&array, &valueToFind, xcCompare_int, &refs);
    if (refs == NULL)
        return TR_allocFailed;
    if (count != 1)
        result = TR_allocFailed;
    else if (xcArrayRefToIdx(&array, refs[0]) != 3)
        result = TR_failure;
    else if (*(int *)refs[0] != 3)
        result = TR_failure;

    free(refs);
    return result;
}

TestResult test_xcArrayFindAll_countZeroMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 5;
    usize count = xcArrayFindAll(&array, &valueToFind, xcCompare_int, NULL);
    if (count != 0)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFindAll_zeroMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);
    TestResult result = TR_success;

    int valueToFind = 5;
    XCRef *refs;
    usize count = xcArrayFindAll(&array, &valueToFind, xcCompare_int, &refs);
    if (refs == NULL)
        return TR_allocFailed;
    if (count != 0)
        result = TR_failure;

    free(refs);
    return result;
}

TestResult test_xcArrayFindAll_countManyMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 3, 2, 3, 3 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int valueToFind = 3;
    usize count = xcArrayFindAll(&array, &valueToFind, xcCompare_int, NULL);
    if (count != 3)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFindAll_manyMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 3, 2, 3, 3 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);
    TestResult result = TR_success;

    int valueToFind = 3;
    XCRef *refs;
    usize count = xcArrayFindAll(&array, &valueToFind, xcCompare_int, &refs);
    if (refs == NULL)
        return TR_allocFailed;
    if (count != 3)
        result = TR_allocFailed;
    else if (xcArrayRefToIdx(&array, refs[0]) != 1)
        result = TR_failure;
    else if (xcArrayRefToIdx(&array, refs[1]) != 3)
        result = TR_failure;
    else if (xcArrayRefToIdx(&array, refs[2]) != 4)
        result = TR_failure;
    else if (*(int *)refs[0] != 3)
        result = TR_failure;
    else if (*(int *)refs[1] != 3)
        result = TR_failure;
    else if (*(int *)refs[2] != 3)
        result = TR_failure;

    free(refs);
    return result;
}

void test_xcArrayFindAll__addTests(void) {
    testSetModule("test_xcArrayFindAll");
    testAdd(TEST(test_xcArrayFindAll_countOneMatch));
    testAdd(TEST(test_xcArrayFindAll_oneMatch));
    testAdd(TEST(test_xcArrayFindAll_countZeroMatches));
    testAdd(TEST(test_xcArrayFindAll_zeroMatches));
    testAdd(TEST(test_xcArrayFindAll_countManyMatches));
    testAdd(TEST(test_xcArrayFindAll_manyMatches));
}
