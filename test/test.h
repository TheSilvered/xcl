#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
// XC_DEBUG_MEM defined in command line
#include "xc.h"

#define TEST(func) (Test) { #func, func }

typedef enum TestResult {
    TR_failure,
    TR_success,
    TR_allocFailed,
    TR_timedOut,
    TR_ignored
} TestResult;

typedef TestResult (*TestFunc)(void);

typedef struct Test {
    const char *name;
    TestFunc func;
} Test;

typedef struct TestModule {
    const char *name;
    Test *tests;
    usize testCount;
} TestModule;

void setModule(const char *name);
void addTest(Test test);

#endif // !TEST_H
