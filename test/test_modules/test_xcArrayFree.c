#include "../test_lib/test.h"

static void test_xcArrayFree__freeRef(XCRef ref) {
    free(*(int **)ref);
}

TestResult test_xcArrayFree_empty(void) {
    XCArray *array = xcArrayNew(sizeof(int *), 10);
    if (!array)
        return TR_allocFailed;

    xcArrayFree(array, test_xcArrayFree__freeRef);
    return TR_success;
}

TestResult test_xcArrayFree_full(void) {
    XCArray *array = xcArrayNew(sizeof(int *), 10);
    if (!array)
        return TR_allocFailed;

    for (int i = 0; i < 10; i++) {
        int *num = malloc(sizeof(int));
        if (!num) {
            xcArrayFree(array, test_xcArrayFree__freeRef);
            return TR_allocFailed;
        }
        xcArrayAppend(array, &num);
    }

    xcArrayFree(array, test_xcArrayFree__freeRef);
    return TR_success;
}

void test_xcArrayFree__addTests(void) {
    testSetModule("test_xcArrayFree");
    testAdd(TEST(test_xcArrayFree_empty));
    testAdd(TEST(test_xcArrayFree_full));
}
