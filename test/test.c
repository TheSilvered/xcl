#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "test.h"
#include "all_tests.h"

#ifndef TEST_TIMEOUT_S
#define TEST_TIMEOUT_S 10
#endif

#ifdef TEST_NO_COLOR

#define RED(s) s
#define GREEN(s) s
#define YELLOW(s) s
#define CYAN(s) s

#else

#define RED(s) "\x1b[91m" s "\x1b[0m"
#define GREEN(s) "\x1b[92m" s "\x1b[0m"
#define YELLOW(s) "\x1b[93m" s "\x1b[0m"
#define CYAN(s) "\x1b[96m" s "\x1b[0m"

#endif

#define LOG_FAILURE(msg, ...) fprintf(stderr, RED("FAILURE") " - "  msg "\n", ## __VA_ARGS__)

static struct {
    usize count;
    TestModule *list;
    TestModule *current;
} modules = { 0 };

static struct {
    clock_t startTime;
    bool result;
    bool running;
} currentTest = { 0 };

static void sigChldHandler(int sig);
static _Noreturn void *threadRoutine(void *func);
static bool runTest(Test test);
static void destroyModules(void);
static const char *parseArguments(int argc, char **argv);
static Test *findTest(const char *name);
static TestModule *findModule(const char *name);
static void runModule(TestModule module);

int main(int argc, char **argv) {
    test_xcArrayInit__addTests();

    const char *specifiedTest = parseArguments(argc, argv);
    if (!specifiedTest) {
        for (usize i = 0; i < modules.count; i++)
            runModule(modules.list[i]);
        goto cleanup;
    }

    Test *testFound = findTest(specifiedTest);
    if (testFound) {
        runTest(*testFound);
        goto cleanup;
    }
    TestModule *moduleFound = findModule(specifiedTest);
    if (!moduleFound) {
        LOG_FAILURE("No test or module matches '%s'", specifiedTest);
        goto cleanup;
    }
    runModule(*moduleFound);

cleanup:
    destroyModules();
    return 0;
}

static _Noreturn void *threadRoutine(void *func) {
    signal(SIGCHLD, sigChldHandler);
    currentTest.result = ((bool (*)(void))func)();
    currentTest.running = false;
    while (true); // wait for SIGCHLD
}

static void runModule(TestModule module) {
    printf(CYAN("Module '%s' (%zi tests):") "\n", module.name, module.testCount);
    int testsPassed = 0;
    for (usize j = 0; j < module.testCount; j++) {
        if (runTest(module.tests[j]))
            testsPassed++;
    }
    printf(YELLOW("  %d/%zi tests passed.") "\n", testsPassed, module.testCount);
}

static bool runTest(Test test) {
    printf("  Test '%s'... ", test.name);
    pthread_t thread;
    currentTest.startTime = clock();
    currentTest.running = true;
    currentTest.result = false;
    if (pthread_create(&thread, NULL, threadRoutine, test.func) != 0) {
        LOG_FAILURE("Failed to create thread");
        destroyModules();
        exit(1);
    }
    while (((double)(clock() - currentTest.startTime)) / CLOCKS_PER_SEC <= TEST_TIMEOUT_S && currentTest.running) ;
    pthread_kill(thread, SIGCHLD);
    if (currentTest.running) { // test timed out
        printf(RED("TIMED OUT\n"));
        return false;
    } else if (currentTest.result) {
        printf(GREEN("PASSED\n"));
        return true;
    } else {
        printf(RED("FAILED\n"));
        return false;
    }
}

static void sigChldHandler(int sig) {
    pthread_exit(0);
}

static void destroyModules(void) {
    if (!modules.list)
        return;
    for (usize i = 0; i < modules.count; i++) {
        TestModule module = modules.list[i];
        if (module.tests)
            free(module.tests);
    }
    free(modules.list);
}

void setModule(const char *name) {
    TestModule *newModules = realloc(modules.list, (modules.count + 1) * sizeof(TestModule));
    if (!newModules) {
        LOG_FAILURE("Allocation failed when setting module '%s'", name);
        destroyModules();
        exit(1);
    }

    modules.list = newModules;
    modules.current = &modules.list[modules.count];
    modules.count++;
    modules.current->name = name;
    modules.current->testCount = 0;
    modules.current->tests = NULL;
}

void addTest(Test test) {
    if (!modules.current) {
        LOG_FAILURE("Trying to add test '%s' without a module set", test.name);
        destroyModules(); // This should do nothing
        exit(1);
    }
    Test *newTests = realloc(modules.current->tests, (modules.current->testCount + 1) * sizeof(Test));
    if (!newTests) {
        LOG_FAILURE("Allocation failed when adding test '%s' to module '%s'", test.name, modules.current->name);
        destroyModules();
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
        LOG_FAILURE("Too many arguments, usage: ./run_tests [test]");
        destroyModules();
        exit(1);
    }
    return (const char *)argv[1];
}

static Test *findTest(const char *name) {
    char buf[256] = {0};
    char *slash = strchr(name, '/');
    if (!slash)
        return NULL;
    if (strlen(name) + 6 > 256) {
        LOG_FAILURE("Given argument is too big to compare");
        destroyModules();
        exit(2);
    }
    memcpy(buf, name, slash - name);
    buf[slash - name] = '\0';
    TestModule *module = findModule(buf);
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

static TestModule *findModule(const char *name) {
    char buf[256];
    if (strlen(name) + 6 > 256) {
        LOG_FAILURE("Given argument is too big to compare");
        destroyModules();
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
