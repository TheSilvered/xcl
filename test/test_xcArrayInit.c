#include "test.h"
#include <limits.h>

bool test_xcArrayInit_empty(void) {
    XCArray array;
    bool result = xcArrayInit(&array, sizeof(int), 0);
    if (result && !array.data)
        return false;
    return true;
}

bool test_xcArrayInit_tooBig(void) {
    XCArray array;
    bool result = xcArrayInit(&array, sizeof(int), (usize)LLONG_MAX);
    if (result) {
        xcArrayDestroy(&array, NULL);
        return false;
    }
    return true;
}

void test_xcArrayInit__addTests(void) {
    setModule("test_xcArrayInit");
    addTest(TEST(test_xcArrayInit_empty));
    addTest(TEST(test_xcArrayInit_tooBig));
}
