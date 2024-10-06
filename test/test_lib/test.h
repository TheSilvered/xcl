#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>
// XC_DEBUG_MEM defined in command line
#include "xc.h"

#ifndef TEST_TIMEOUT_S
#define TEST_TIMEOUT_S 30
#endif

#ifdef TEST_NO_COLOR

#define RED(s) s
#define GREEN(s) s
#define YELLOW(s) s
#define CYAN(s) s
#define GRAY(s) s

#else

#define RED(s) "\x1b[91m" s "\x1b[0m"
#define GREEN(s) "\x1b[92m" s "\x1b[0m"
#define YELLOW(s) "\x1b[93m" s "\x1b[0m"
#define CYAN(s) "\x1b[96m" s "\x1b[0m"
#define GRAY(s) "\x1b[90m" s "\x1b[0m"

#endif

#define TEST(func) (Test) { #func, func }
#define TEST_LOG_FAILURE(msg, ...) fprintf(stderr, RED("FAILURE") " - "  msg "\n", ## __VA_ARGS__)

#define TR_failure (TestResult) { _TRT_failure, __LINE__ }
#define TR_success (TestResult) { _TRT_success, -1 }
#define TR_allocFailed (TestResult) { _TRT_allocFailed, __LINE__ }
#define TR_timedOut (TestResult) { _TRT_timedOut, -1 }
#define TR_ignored (TestResult) { _TRT_ignored, -1 }

enum _TestResultType {
    _TRT_failure,
    _TRT_success,
    _TRT_allocFailed,
    _TRT_timedOut,
    _TRT_ignored
};

typedef struct TestResult {
    enum _TestResultType _type;
    int _line;
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

// Set the current module.
void testSetModule(const char *name);
// Add a test to the current module.
void testAdd(Test test);

// Get the list of all modules added
TestModule *testGetModules(void);
// Get the number of modules added
usize testGetModuleCount(void);

// Run a module, the number of elements in each category is added to the correct pointer
void testRunModule(TestModule module, int *outPassed, int *outFailed, int *outIgnored);
// Run a test
TestResult testRun(Test test);
// Find a module given its name (e.g. `xcArrayInit` for `test_xcArrayInit.c`), returns `NULL` if it doesn't exist
TestModule *testFindModule(const char *name);
// Find a test given its name (e.g. `xcCompare/uint` for `test_xcCompare_uint`), returns `NULL` if it doesn't exist
Test *testFind(const char *name);
// Unload library
void testExit(void);
// Check if a `TestResult` is considered a failure.
bool testIsFailure(TestResult result);

#endif // !TEST_H
