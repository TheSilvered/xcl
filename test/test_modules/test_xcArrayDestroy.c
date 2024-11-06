#include "test.h"
#include "xc.h"

static void test_xcArrayDestroy__freeRef(XCRef ref) {
    free(*(int **)ref);
}

TestResult test_xcArrayDestroy_empty(void) {
    XCArray array;
    if (!xcArrayInit(&array, sizeof(int *), 10))
        return TR_allocFailed;

    xcArrayDestroy(&array, test_xcArrayDestroy__freeRef);
    return TR_success;
}

TestResult test_xcArrayDestroy_full(void) {
    XCArray array;
    if (!xcArrayInit(&array, sizeof(int *), 10))
        return TR_allocFailed;

    for (int i = 0; i < 10; i++) {
        int *num = malloc(sizeof(int));
        if (!num) {
            xcArrayDestroy(&array, test_xcArrayDestroy__freeRef);
            return TR_allocFailed;
        }
        *num = i;
        xcArrayAppend(&array, &num);
    }

    xcArrayDestroy(&array, test_xcArrayDestroy__freeRef);
    return TR_success;
}

void test_xcArrayDestroy__addTests(void) {
    testSetModule("test_xcArrayDestroy");
    testAdd(TEST(test_xcArrayDestroy_empty));
    testAdd(TEST(test_xcArrayDestroy_full));
}
