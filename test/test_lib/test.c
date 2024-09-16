#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "test.h"
#include "xc.h"

static struct {
    usize count;
    TestModule *list;
    TestModule *current;
} modules = { 0 };

static struct {
    clock_t startTime;
    TestResult result;
    bool running;
} currentTest = { 0 };

static void sigChldHandler(int sig);
static _Noreturn void *threadRoutine(void *func);
static const char *parseArguments(int argc, char **argv);

static _Noreturn void *threadRoutine(void *func) {
    signal(SIGCHLD, sigChldHandler);
    currentTest.result = ((TestFunc)func)();
    currentTest.running = false;
    while (true); // wait for SIGCHLD
}

void testRunModule(TestModule module, int *outPassed, int *outFailed, int *outIgnored) {
    printf(CYAN("Module '%s' (%zi tests):") "\n", module.name, module.testCount);
    int testsPassed = 0;
    int testsFailed = 0;
    int testsIgnored = 0;
    for (usize j = 0; j < module.testCount; j++) {
        switch (testRun(module.tests[j])) {
        case TR_success:
            testsPassed++;
            break;
        case TR_ignored:
            testsIgnored++;
            break;
        case TR_failure:
        case TR_timedOut:
        case TR_allocFailed:
            testsFailed++;
        }
    }
    printf(YELLOW("  %d passed, %d failed, %d ignored.") "\n", testsPassed, testsFailed, testsIgnored);
    if (outPassed) *outPassed += testsPassed;
    if (outFailed) *outFailed += testsFailed;
    if (outIgnored) *outIgnored += testsIgnored;
}

TestResult testRun(Test test) {
    printf("  Test '%s'... ", test.name);
    pthread_t thread;
    currentTest.startTime = clock();
    currentTest.running = true;
    currentTest.result = false;
    if (pthread_create(&thread, NULL, threadRoutine, test.func) != 0) {
        TEST_LOG_FAILURE("Failed to create thread");
        testExit();
        exit(1);
    }
    while (((double)(clock() - currentTest.startTime)) / CLOCKS_PER_SEC <= TEST_TIMEOUT_S && currentTest.running) ;
    pthread_kill(thread, SIGCHLD);
    if (currentTest.running)
        currentTest.result = TR_timedOut;

    switch (currentTest.result) {
    case TR_success:
        printf(GREEN("PASSED\n"));
        break;
    case TR_failure:
        printf(RED("FAILED\n"));
        break;
    case TR_ignored:
        printf(GRAY("IGNORED\n"));
        break;
    case TR_allocFailed:
        printf(RED("MEMORY ALLOCATION FAILED\n"));
        break;
    case TR_timedOut:
        printf(RED("TIMED OUT\n"));
        break;
    }

    return currentTest.result;
}

static void sigChldHandler(int sig) {
    pthread_exit(0);
}

void testExit(void) {
    if (!modules.list)
        return;
    for (usize i = 0; i < modules.count; i++) {
        TestModule module = modules.list[i];
        if (module.tests)
            free(module.tests);
    }
    free(modules.list);
}

bool testIsFailure(TestResult result) {
    switch (result) {
    case TR_success:
    case TR_ignored:
        return false;
    case TR_failure:
    case TR_timedOut:
    case TR_allocFailed:
        return true;
    }
}

void testSetModule(const char *name) {
    TestModule *newModules = realloc(modules.list, (modules.count + 1) * sizeof(TestModule));
    if (!newModules) {
        TEST_LOG_FAILURE("Allocation failed when setting module '%s'", name);
        testExit();
        exit(1);
    }

    modules.list = newModules;
    modules.current = &modules.list[modules.count];
    modules.count++;
    modules.current->name = name;
    modules.current->testCount = 0;
    modules.current->tests = NULL;
}

void testAdd(Test test) {
    if (!modules.current) {
        TEST_LOG_FAILURE("Trying to add test '%s' without a module set", test.name);
        testExit(); // This should do nothing
        exit(1);
    }
    Test *newTests = realloc(modules.current->tests, (modules.current->testCount + 1) * sizeof(Test));
    if (!newTests) {
        TEST_LOG_FAILURE("Allocation failed when adding test '%s' to module '%s'", test.name, modules.current->name);
        testExit();
        exit(1);
    }
    modules.current->tests = newTests;
    modules.current->tests[modules.current->testCount] = test;
    modules.current->testCount++;
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

Test *testFind(const char *name) {
    char buf[256] = {0};
    char *slash = strchr(name, '/');
    if (!slash)
        return NULL;
    if (strlen(name) + 6 > 256) {
        TEST_LOG_FAILURE("Given argument is too big to compare");
        testExit();
        exit(2);
    }
    memcpy(buf, name, slash - name);
    buf[slash - name] = '\0';
    TestModule *module = testFindModule(buf);
    if (!module)
        return NULL;

    memcpy(buf, "test_", 5);
    memcpy(buf + 5, name, strlen(name));
    buf[5 + (slash - name)] = '_';

    printf("Searching for test '%s'...\n", buf);
    for (usize i = 0; i < module->testCount; i++) {
        Test *test = &module->tests[i];
        if (strcmp(buf, test->name) == 0)
            return test;
    }
    return 0;
}

TestModule *testFindModule(const char *name) {
    char buf[256];
    if (strlen(name) + 6 > 256) {
        TEST_LOG_FAILURE("Given argument is too big to compare");
        testExit();
        exit(2);
    }
    memcpy(buf, "test_", 5);
    strcpy(buf + 5, name);
    printf("Searching for module '%s'...\n", buf);
    for (usize i = 0; i < modules.count; i++) {
        TestModule *module = &modules.list[i];
        if (strcmp(buf, module->name) == 0)
            return module;
    }
    return NULL;
}

TestModule *testGetModules(void) {
    return modules.list;
}

usize testGetModuleCount(void) {
    return modules.count;
}
