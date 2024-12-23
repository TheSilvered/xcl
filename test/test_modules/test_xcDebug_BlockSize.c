#include "test.h"
#include "xc.h"

TestResult test_xcDebug_BlockSize_malloc(void) {
    XCMemBlock block = malloc(10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10) {
        free(block);
        return TR_failure;
    }
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_mallocEmpty(void) {
    XCMemBlock block = malloc(0);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 0) {
        free(block);
        return TR_failure;
    }
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_calloc(void) {
    XCMemBlock block = calloc(10, 1);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10) {
        free(block);
        return TR_failure;
    }
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_callocEmpty(void) {
    XCMemBlock block = calloc(0, 1);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 0) {
        free(block);
        return TR_failure;
    }
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_reallocFromNull(void) {
    XCMemBlock block = realloc(NULL, 10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10) {
        free(block);
        return TR_failure;
    }
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_reallocFromBlock(void) {
    XCMemBlock block = malloc(10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10) {
        free(block);
        return TR_failure;
    }
    XCMemBlock newBlock = realloc(block, 20);
    if (!newBlock) {
        free(block);
        return TR_allocFailed;
    }
    if (xcDebug_BlockSize(newBlock) != 20) {
        free(block);
        return TR_failure;
    }
    free(newBlock);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_reallocToEmpty(void) {
    XCMemBlock block = malloc(10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10) {
        free(block);
        return TR_failure;
    }
    XCMemBlock newBlock = realloc(block, 0);
    if (!newBlock) {
        free(block);
        return TR_allocFailed;
    }
    if (xcDebug_BlockSize(newBlock) != 0) {
        free(block);
        return TR_failure;
    }
    free(newBlock);
    return TR_success;
}

void test_xcDebug_BlockSize__addTests(void) {
    testSetModule("test_xcDebug_BlockSize");
    testAdd(TEST(test_xcDebug_BlockSize_malloc));
    testAdd(TEST(test_xcDebug_BlockSize_mallocEmpty));
    testAdd(TEST(test_xcDebug_BlockSize_calloc));
    testAdd(TEST(test_xcDebug_BlockSize_callocEmpty));
    testAdd(TEST(test_xcDebug_BlockSize_reallocFromNull));
    testAdd(TEST(test_xcDebug_BlockSize_reallocFromBlock));
    testAdd(TEST(test_xcDebug_BlockSize_reallocToEmpty));
}
