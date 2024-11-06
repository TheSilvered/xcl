#include "test.h"
#include "xc.h"

static bool filter(XCRef value) {
    return *(int *)value == 3;
}

TestResult test_xcArrayFilter_countOneMatch(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    usize count = xcArrayFilter(&array, filter, NULL);
    if (count != 1)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFilter_oneMatch(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);
    TestResult result = TR_success;

    XCRef *refs;
    usize count = xcArrayFilter(&array, filter, &refs);
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

TestResult test_xcArrayFilter_countZeroMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 5, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    usize count = xcArrayFilter(&array, filter, NULL);
    if (count != 0)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFilter_zeroMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 5, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);
    TestResult result = TR_success;

    XCRef *refs;
    usize count = xcArrayFilter(&array, filter, &refs);
    if (refs == NULL)
        return TR_allocFailed;
    if (count != 0)
        result = TR_failure;

    free(refs);
    return result;
}

TestResult test_xcArrayFilter_countManyMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 3, 2, 3, 3 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    usize count = xcArrayFilter(&array, filter, NULL);
    if (count != 3)
        return TR_failure;

    return TR_success;
}

TestResult test_xcArrayFilter_manyMatches(void) {
    XCArray array;
    int arrValues[] = { 0, 3, 2, 3, 3 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);
    TestResult result = TR_success;

    XCRef *refs;
    usize count = xcArrayFilter(&array, filter, &refs);
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

void test_xcArrayFilter__addTests(void) {
    testSetModule("test_xcArrayFilter");
    testAdd(TEST(test_xcArrayFilter_countOneMatch));
    testAdd(TEST(test_xcArrayFilter_oneMatch));
    testAdd(TEST(test_xcArrayFilter_countZeroMatches));
    testAdd(TEST(test_xcArrayFilter_zeroMatches));
    testAdd(TEST(test_xcArrayFilter_countManyMatches));
    testAdd(TEST(test_xcArrayFilter_manyMatches));
}
