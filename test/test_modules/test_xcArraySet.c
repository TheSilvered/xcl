#include "test.h"
#include "xc.h"

TestResult test_xcArraySet_positiveInBounds(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    for (int i = 0; i < 5; i++) {
        int value = i + 10;
        if (!xcArraySet(&array, &value, i, NULL))
            return TR_failure;
    }

    for (int i = 0; i < 5; i++) {
        if (*(int *)xcArrayGet(&array, i) != i + 10)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArraySet_negativeInBounds(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    for (int i = -1; i <= -5; i--) {
        int value = i + 20;
        if (!xcArraySet(&array, &value, i, NULL))
            return TR_failure;
    }

    for (int i = -1; i <= -5; i--) {
        if (*(int *)xcArrayGet(&array, i) != i + 20)
            return TR_failure;
    }
    return TR_success;
}

TestResult test_xcArraySet_outOfBounds(void) {
    XCArray array;
    int arrValues[] = { 0, 1, 2, 3, 4 };
    xcArrayInitFromData(&array, sizeof(int), sizeof(arrValues) / sizeof(int), arrValues);

    int value = 10;
    if (xcArraySet(&array, &value, 5, NULL))
        return TR_failure;
    if (xcArraySet(&array, &value, -6, NULL))
        return TR_failure;
    return TR_success;
}

static void test_xcArraySet_freeRef(XCRef val) {
    free(*(int **)val);
}

TestResult test_xcArraySet_withDestructor(void) {
    XCArray array;
    if (!xcArrayInit(&array, sizeof(int *), 5))
        return TR_allocFailed;

    for (int i = 0; i < 5; i++) {
        int *value = malloc(sizeof(int));
        if (!value) {
            xcArrayDestroy(&array, test_xcArraySet_freeRef);
            return TR_allocFailed;
        }
        *value = i;
        xcArrayAppend(&array, &value);
    }

    for (int i = 0; i < 5; i++) {
        int *value = malloc(sizeof(int));
        if (!value) {
            xcArrayDestroy(&array, test_xcArraySet_freeRef);
            return TR_allocFailed;
        }
        *value = i + 10;
        if (!xcArraySet(&array, &value, i, test_xcArraySet_freeRef)) {
            xcArrayDestroy(&array, test_xcArraySet_freeRef);
            return TR_failure;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (**(int **)xcArrayGet(&array, i) != i + 10) {
            xcArrayDestroy(&array, test_xcArraySet_freeRef);
            return TR_failure;
        }
    }

    xcArrayDestroy(&array, test_xcArraySet_freeRef);
    return TR_success;
}

void test_xcArraySet__addTests(void) {
    testSetModule("test_xcArraySet");
    testAdd(TEST(test_xcArraySet_positiveInBounds));
    testAdd(TEST(test_xcArraySet_negativeInBounds));
    testAdd(TEST(test_xcArraySet_outOfBounds));
    testAdd(TEST(test_xcArraySet_withDestructor));
}
