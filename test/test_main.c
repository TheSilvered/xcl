#include "test_lib/test.h"

#include "test_modules/test_xcCompare.c"
#include "test_modules/test_xcDebug_BlockSize.c"
#include "test_modules/test_xcArrayInit.c"
#include "test_modules/test_xcArrayInitFromCopy.c"
#include "test_modules/test_xcArrayInitFromData.c"
#include "test_modules/test_xcArrayNew.c"
#include "test_modules/test_xcArrayNewFromCopy.c"
#include "test_modules/test_xcArrayNewFromData.c"
#include "test_modules/test_xcArrayDestroy.c"
#include "test_modules/test_xcArrayFree.c"
#include "test_modules/test_xcArrayGet.c"
#include "test_modules/test_xcArrayFind.c"
#include "test_modules/test_xcArrayFindAll.c"
#include "test_modules/test_xcArrayFilter.c"
#include "test_modules/test_xcArrayAppend.c"
#include "test_modules/test_xcArrayInsert.c"
#include "test_modules/test_xcArrayExtend.c"
#include "test_modules/test_xcArraySet.c"
#include "test_modules/test_xcArraySwap.c"

static const char *parseArguments(int argc, char **argv);
static bool runAllModules(void);

int main(int argc, char **argv) {
    test_xcCompare__addTests();
    test_xcDebug_BlockSize__addTests();
    test_xcArrayInit__addTests();
    test_xcArrayInitFromCopy__addTests();
    test_xcArrayInitFromData__addTests();
    test_xcArrayNew__addTests();
    test_xcArrayNewFromCopy__addTests();
    test_xcArrayNewFromData__addTests();
    test_xcArrayDestroy__addTests();
    test_xcArrayFree__addTests();
    test_xcArrayGet__addTests();
    test_xcArrayFind__addTests();
    test_xcArrayFindAll__addTests();
    test_xcArrayFilter__addTests();
    test_xcArrayAppend__addTests();
    test_xcArrayInsert__addTests();
    test_xcArrayExtend__addTests();
    test_xcArraySet__addTests();
    test_xcArraySwap__addTests();

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
