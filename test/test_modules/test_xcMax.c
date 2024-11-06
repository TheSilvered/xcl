#include "test.h"
#include "xc.h"

TestResult test_xcMax_int(void) {
    if (xcMax_int(2, 4) != 4)
        return TR_failure;
    if (xcMax_int(4, 2) != 4)
        return TR_failure;
    if (xcMax_int(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_uint(void) {
    if (xcMax_uint(2, 4) != 4)
        return TR_failure;
    if (xcMax_uint(4, 2) != 4)
        return TR_failure;
    if (xcMax_uint(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_i8(void) {
    if (xcMax_i8(2, 4) != 4)
        return TR_failure;
    if (xcMax_i8(4, 2) != 4)
        return TR_failure;
    if (xcMax_i8(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_u8(void) {
    if (xcMax_u8(2, 4) != 4)
        return TR_failure;
    if (xcMax_u8(4, 2) != 4)
        return TR_failure;
    if (xcMax_u8(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_i16(void) {
    if (xcMax_i16(2, 4) != 4)
        return TR_failure;
    if (xcMax_i16(4, 2) != 4)
        return TR_failure;
    if (xcMax_i16(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_u16(void) {
    if (xcMax_u16(2, 4) != 4)
        return TR_failure;
    if (xcMax_u16(4, 2) != 4)
        return TR_failure;
    if (xcMax_u16(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_i32(void) {
    if (xcMax_i32(2, 4) != 4)
        return TR_failure;
    if (xcMax_i32(4, 2) != 4)
        return TR_failure;
    if (xcMax_i32(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_u32(void) {
    if (xcMax_u32(2, 4) != 4)
        return TR_failure;
    if (xcMax_u32(4, 2) != 4)
        return TR_failure;
    if (xcMax_u32(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_i64(void) {
    if (xcMax_i64(2, 4) != 4)
        return TR_failure;
    if (xcMax_i64(4, 2) != 4)
        return TR_failure;
    if (xcMax_i64(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_u64(void) {
    if (xcMax_u64(2, 4) != 4)
        return TR_failure;
    if (xcMax_u64(4, 2) != 4)
        return TR_failure;
    if (xcMax_u64(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_isize(void) {
    if (xcMax_isize(2, 4) != 4)
        return TR_failure;
    if (xcMax_isize(4, 2) != 4)
        return TR_failure;
    if (xcMax_isize(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_usize(void) {
    if (xcMax_usize(2, 4) != 4)
        return TR_failure;
    if (xcMax_usize(4, 2) != 4)
        return TR_failure;
    if (xcMax_usize(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_f32(void) {
    if (xcMax_f32(2.0f, 4.0f) != 4.0f)
        return TR_failure;
    if (xcMax_f32(4.0f, 2.0f) != 4.0f)
        return TR_failure;
    if (xcMax_f32(4.0f, 4.0f) != 4.0f)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_f64(void) {
    if (xcMax_f64(2.0, 4.0) != 4.0)
        return TR_failure;
    if (xcMax_f64(4.0, 2.0) != 4.0)
        return TR_failure;
    if (xcMax_f64(4.0, 4.0) != 4.0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMax_general(void) {
    int a = 2;
    int b = 4;
    int c = 4;
    if (xcMax(&a, &b, xcCompare_int) != &b)
        return TR_failure;
    if (xcMax(&b, &a, xcCompare_int) != &b)
        return TR_failure;
    if (xcMax(&b, &c, xcCompare_int) != &b)
        return TR_failure;
    if (xcMax(&c, &b, xcCompare_int) != &c)
        return TR_failure;
    return TR_success;
}

void test_xcMax__addTests(void) {
    testSetModule("test_xcMax");
    testSetModule("test_xcMin");
    testAdd(TEST(test_xcMax_int));
    testAdd(TEST(test_xcMax_uint));
    testAdd(TEST(test_xcMax_i8));
    testAdd(TEST(test_xcMax_u8));
    testAdd(TEST(test_xcMax_i16));
    testAdd(TEST(test_xcMax_u16));
    testAdd(TEST(test_xcMax_i32));
    testAdd(TEST(test_xcMax_u32));
    testAdd(TEST(test_xcMax_i64));
    testAdd(TEST(test_xcMax_u64));
    testAdd(TEST(test_xcMax_isize));
    testAdd(TEST(test_xcMax_usize));
    testAdd(TEST(test_xcMax_f32));
    testAdd(TEST(test_xcMax_f64));
    testAdd(TEST(test_xcMax_general));
}
