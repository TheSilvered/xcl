#include "test.h"
#include "xc.h"

TestResult test_xcArrayInitFromCopy_empty(void) {
    XCArray array;
    int cArr[] = { 1 };
    TestResult result = TR_success;
    if (!xcArrayInitFromCopy(&array, sizeof(int), 0, cArr))
        return TR_allocFailed;
    if (array.len != 0 || array.cap != 0)
        result = TR_failure;

    xcArrayDestroy(&array, NULL);
    return result;
}

TestResult test_xcArrayInitFromCopy_full(void) {
    XCArray array;
    int cArr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TestResult result = TR_success;
    const usize arrLength = sizeof(cArr) / sizeof(int);

    if (!xcArrayInitFromCopy(&array, sizeof(int), arrLength, cArr))
        return TR_allocFailed;
    if (array.len != arrLength || array.cap != arrLength)
        result = TR_failure;
    else if (xcDebug_BlockSize(array.data) != arrLength * sizeof(int))
        result = TR_failure;
    else {
        for (int i = 0; i < arrLength; i++) {
            if (*(int *)xcArrayGet(&array, i) != i + 1) {
                result = TR_failure;
                break;
            }
        }
    }
    xcArrayDestroy(&array, NULL);
    return result;
}

void test_xcArrayInitFromCopy__addTests(void) {
    testSetModule("test_xcArrayInitFromCopy");
    testAdd(TEST(test_xcArrayInitFromCopy_empty));
    testAdd(TEST(test_xcArrayInitFromCopy_full));
}
