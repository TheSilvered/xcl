#include "test_lib/test.h"

#include "test_modules/all_modules.c"

static const char *parseArguments(int argc, char **argv);
static bool runAllModules(void);

int main(int argc, char **argv) {
    addTests();
    int returnValue = 0;
    const char *specifiedTest = parseArguments(argc, argv);
    if (!specifiedTest) {
        returnValue = runAllModules() ? 0 : 1;
        goto cleanup;
    }

    Test *testFound = testFind(specifiedTest);
    if (testFound) {
        returnValue = testIsFailure(testRun(*testFound)) ? 1 : 0;
        goto cleanup;
    }
    TestModule *moduleFound = testFindModule(specifiedTest);
    if (!moduleFound) {
        TEST_LOG_FAILURE("No test or module matches '%s'", specifiedTest);
        returnValue = 1;
        goto cleanup;
    }
    testRunModule(*moduleFound, NULL, &returnValue, NULL);
    if (returnValue > 0)
        returnValue = 1;

cleanup:
    testExit();
    return returnValue;
}

static const char *parseArguments(int argc, char **argv) {
    if (argc < 2)
        return NULL;
    else if (argc > 2) {
        TEST_LOG_FAILURE("Too many arguments, usage: ./run_tests [test]");
        testExit();
        exit(1);
    }
    return (const char *)argv[1];
}

static bool runAllModules(void) {
    int passed = 0;
    int failed = 0;
    int ignored = 0;
    TestModule *modules = testGetModules();
    usize moduleCount = testGetModuleCount();
    for (usize i = 0; i < moduleCount; i++)
        testRunModule(modules[i], &passed, &failed, &ignored);
    printf(GRAY("%d passed, %d failed and %d ignored.") "\n", passed, failed, ignored);
    return failed == 0;
}
