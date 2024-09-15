#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include "xc.h"

#define TEST(func) (Test) { #func, func }

typedef struct Test {
    const char *name;
    bool (*func)(void);
} Test;

typedef struct TestModule {
    const char *name;
    Test *tests;
    usize testCount;
} TestModule;

void setModule(const char *name);
void addTest(Test test);

#endif // !TEST_H
