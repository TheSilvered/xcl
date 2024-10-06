#include <stdio.h>
#include <string.h>
#include "test.h"
#include "xc.h"

#ifndef TEST_NO_THREADS

#include <signal.h>
#include <time.h>

static struct {
    clock_t startTime;
    TestResult result;
    bool running;
} currentTest = { 0 };

#ifdef _WIN32

#include <Windows.h>

static DWORD threadRoutine(void *func) {
    currentTest.result = ((TestFunc)func)();
    currentTest.running = false;
    while (true); // wait for TerminateThread
}

static TestResult threadedRun(TestFunc func) {
    currentTest.startTime = clock();
    currentTest.running = true;
    currentTest.result = false;
    HANDLE hThread = CreateThread(NULL, 0, threadRoutine, func, 0, NULL);
    if (!hThread) {
        TEST_LOG_FAILURE("Failed to create thread");
        testExit();
        exit(1);
    }

    while (((double)(clock() - currentTest.startTime)) / CLOCKS_PER_SEC <= TEST_TIMEOUT_S && currentTest.running) ;
    if (!TerminateThread(hThread, 0)) {
        TEST_LOG_FAILURE("Failed to kill thread.");
        testExit();
        exit(1);
    }
    if (currentTest.running)
        currentTest.result = TR_timedOut;
    return currentTest.result;
}

#else

#include <pthread.h>

static void sigChldHandler(int sig) {
    pthread_exit(0);
}

static void *threadRoutine(void *func) {
    signal(SIGCHLD, sigChldHandler);
    currentTest.result = ((TestFunc)func)();
    currentTest.running = false;
    while (true); // wait for SIGCHLD
}

static TestResult threadedRun(TestFunc func) {
    pthread_t thread;
    currentTest.startTime = clock();
    currentTest.running = true;
    currentTest.result = TR_ignored;
    if (pthread_create(&thread, NULL, threadRoutine, func) != 0) {
        TEST_LOG_FAILURE("Failed to create thread");
        testExit();
        exit(1);
    }
    while (((double)(clock() - currentTest.startTime)) / CLOCKS_PER_SEC <= TEST_TIMEOUT_S && currentTest.running) ;
    pthread_kill(thread, SIGCHLD);
    if (currentTest.running)
        currentTest.result = TR_timedOut;
    return currentTest.result;
}

#endif // !_WIN32

#endif // !TEST_NO_THREADS

static struct {
    usize count;
    TestModule *list;
    TestModule *current;
} modules = { 0 };

void testRunModule(TestModule module, int *outPassed, int *outFailed, int *outIgnored) {
    printf(CYAN("Module '%s' (%zi tests):") "\n", module.name, module.testCount);
    fflush(stdout);
    int testsPassed = 0;
    int testsFailed = 0;
    int testsIgnored = 0;
    for (usize j = 0; j < module.testCount; j++) {
        switch (testRun(module.tests[j])._type) {
        case _TRT_success:
            testsPassed++;
            break;
        case _TRT_ignored:
            testsIgnored++;
            break;
        case _TRT_failure:
        case _TRT_timedOut:
        case _TRT_allocFailed:
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
    fflush(stdout);

#ifndef TEST_NO_THREADS
    TestResult result = threadedRun(test.func);
#else
    TestResult result = test.func();
#endif
    switch (result._type) {
    case _TRT_success:
        printf(GREEN("PASSED\n"));
        break;
    case _TRT_failure:
        printf(RED("FAILED ") "(line %d)\n", result._line);
        break;
    case _TRT_ignored:
        printf(GRAY("IGNORED\n"));
        break;
    case _TRT_allocFailed:
        printf(RED("MEMORY ALLOCATION FAILED ") "(line %d)\n", result._line);
        break;
    case _TRT_timedOut:
        printf(RED("TIMED OUT\n"));
        break;
    }
    fflush(stdout);
    return result;
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
    switch (result._type) {
    case _TRT_success:
    case _TRT_ignored:
        return false;
    case _TRT_failure:
    case _TRT_timedOut:
    case _TRT_allocFailed:
        return true;
    }
    return false;
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
    fflush(stdout);
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
