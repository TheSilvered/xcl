#include "test.h"

TestResult test_xcDebug_BlockSize_malloc(void) {
    void *block = malloc(10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10)
        return TR_failure;
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_mallocEmpty(void) {
    void *block = malloc(0);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 0)
        return TR_failure;
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_calloc(void) {
    void *block = calloc(10, 1);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10)
        return TR_failure;
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_callocEmpty(void) {
    void *block = calloc(0, 1);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 0)
        return TR_failure;
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_reallocFromNull(void) {
    void *block = realloc(NULL, 10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10)
        return TR_failure;
    free(block);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_reallocFromBlock(void) {
    void *block = malloc(10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10)
        return TR_failure;
    void *newBlock = realloc(block, 20);
    if (!newBlock) {
        free(block);
        return TR_allocFailed;
    }
    if (xcDebug_BlockSize(newBlock) != 20)
        return TR_failure;
    free(newBlock);
    return TR_success;
}

TestResult test_xcDebug_BlockSize_reallocToEmpty(void) {
    void *block = malloc(10);
    if (!block)
        return TR_allocFailed;
    if (xcDebug_BlockSize(block) != 10)
        return TR_failure;
    void *newBlock = realloc(block, 0);
    if (!newBlock) {
        free(block);
        return TR_allocFailed;
    }
    if (xcDebug_BlockSize(newBlock) != 0)
        return TR_failure;
    free(newBlock);
    return TR_success;
}

void test_xcDebug_BlockSize__addTests(void) {
    setModule("test_xcDebug_BlockSize");
    addTest(TEST(test_xcDebug_BlockSize_malloc));
    addTest(TEST(test_xcDebug_BlockSize_mallocEmpty));
    addTest(TEST(test_xcDebug_BlockSize_calloc));
    addTest(TEST(test_xcDebug_BlockSize_callocEmpty));
    addTest(TEST(test_xcDebug_BlockSize_reallocFromNull));
    addTest(TEST(test_xcDebug_BlockSize_reallocFromBlock));
    addTest(TEST(test_xcDebug_BlockSize_reallocToEmpty));
}
