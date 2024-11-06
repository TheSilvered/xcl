#include "test.h"
#include "xc.h"

TestResult test_xcMin_int(void) {
    if (xcMin_int(2, 4) != 2)
        return TR_failure;
    if (xcMin_int(4, 2) != 2)
        return TR_failure;
    if (xcMin_int(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_uint(void) {
    if (xcMin_uint(2, 4) != 2)
        return TR_failure;
    if (xcMin_uint(4, 2) != 2)
        return TR_failure;
    if (xcMin_uint(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_i8(void) {
    if (xcMin_i8(2, 4) != 2)
        return TR_failure;
    if (xcMin_i8(4, 2) != 2)
        return TR_failure;
    if (xcMin_i8(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_u8(void) {
    if (xcMin_u8(2, 4) != 2)
        return TR_failure;
    if (xcMin_u8(4, 2) != 2)
        return TR_failure;
    if (xcMin_u8(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_i16(void) {
    if (xcMin_i16(2, 4) != 2)
        return TR_failure;
    if (xcMin_i16(4, 2) != 2)
        return TR_failure;
    if (xcMin_i16(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_u16(void) {
    if (xcMin_u16(2, 4) != 2)
        return TR_failure;
    if (xcMin_u16(4, 2) != 2)
        return TR_failure;
    if (xcMin_u16(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_i32(void) {
    if (xcMin_i32(2, 4) != 2)
        return TR_failure;
    if (xcMin_i32(4, 2) != 2)
        return TR_failure;
    if (xcMin_i32(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_u32(void) {
    if (xcMin_u32(2, 4) != 2)
        return TR_failure;
    if (xcMin_u32(4, 2) != 2)
        return TR_failure;
    if (xcMin_u32(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_i64(void) {
    if (xcMin_i64(2, 4) != 2)
        return TR_failure;
    if (xcMin_i64(4, 2) != 2)
        return TR_failure;
    if (xcMin_i64(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_u64(void) {
    if (xcMin_u64(2, 4) != 2)
        return TR_failure;
    if (xcMin_u64(4, 2) != 2)
        return TR_failure;
    if (xcMin_u64(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_isize(void) {
    if (xcMin_isize(2, 4) != 2)
        return TR_failure;
    if (xcMin_isize(4, 2) != 2)
        return TR_failure;
    if (xcMin_isize(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_usize(void) {
    if (xcMin_usize(2, 4) != 2)
        return TR_failure;
    if (xcMin_usize(4, 2) != 2)
        return TR_failure;
    if (xcMin_usize(4, 4) != 4)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_f32(void) {
    if (xcMin_f32(2.0f, 4.0f) != 2.0f)
        return TR_failure;
    if (xcMin_f32(4.0f, 2.0f) != 2.0f)
        return TR_failure;
    if (xcMin_f32(4.0f, 4.0f) != 4.0f)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_f64(void) {
    if (xcMin_f64(2.0, 4.0) != 2.0)
        return TR_failure;
    if (xcMin_f64(4.0, 2.0) != 2.0)
        return TR_failure;
    if (xcMin_f64(4.0, 4.0) != 4.0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcMin_general(void) {
    int a = 2;
    int b = 4;
    int c = 2;
    if (xcMin(&a, &b, xcCompare_int) != &a)
        return TR_failure;
    if (xcMin(&b, &a, xcCompare_int) != &a)
        return TR_failure;
    if (xcMin(&a, &c, xcCompare_int) != &a)
        return TR_failure;
    if (xcMin(&c, &a, xcCompare_int) != &c)
        return TR_failure;
    return TR_success;
}

void test_xcMin__addTests(void) {
    testSetModule("test_xcMin");
    testAdd(TEST(test_xcMin_int));
    testAdd(TEST(test_xcMin_uint));
    testAdd(TEST(test_xcMin_i8));
    testAdd(TEST(test_xcMin_u8));
    testAdd(TEST(test_xcMin_i16));
    testAdd(TEST(test_xcMin_u16));
    testAdd(TEST(test_xcMin_i32));
    testAdd(TEST(test_xcMin_u32));
    testAdd(TEST(test_xcMin_i64));
    testAdd(TEST(test_xcMin_u64));
    testAdd(TEST(test_xcMin_isize));
    testAdd(TEST(test_xcMin_usize));
    testAdd(TEST(test_xcMin_f32));
    testAdd(TEST(test_xcMin_f64));
    testAdd(TEST(test_xcMin_general));
}
