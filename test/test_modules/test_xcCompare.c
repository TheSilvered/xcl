#include "../test_lib/test.h"

TestResult test_xcCompare_int(void) {
    int a = 5, b = 2, c = 5;
    if (xcCompare_int(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_int(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_int(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_int(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_uint(void) {
    uint a = 5, b = 2, c = 5;
    if (xcCompare_uint(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_uint(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_uint(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_uint(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_i8(void) {
    i8 a = 5, b = 2, c = 5;
    if (xcCompare_i8(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_i8(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_i8(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_i8(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_u8(void) {
    u8 a = 5, b = 2, c = 5;
    if (xcCompare_u8(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_u8(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_u8(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_u8(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_i16(void) {
    i16 a = 5, b = 2, c = 5;
    if (xcCompare_i16(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_i16(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_i16(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_i16(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_u16(void) {
    u16 a = 5, b = 2, c = 5;
    if (xcCompare_u16(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_u16(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_u16(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_u16(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_i32(void) {
    i32 a = 5, b = 2, c = 5;
    if (xcCompare_i32(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_i32(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_i32(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_i32(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_u32(void) {
    u32 a = 5, b = 2, c = 5;
    if (xcCompare_u32(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_u32(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_u32(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_u32(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_i64(void) {
    i64 a = 5, b = 2, c = 5;
    if (xcCompare_i64(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_i64(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_i64(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_i64(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_u64(void) {
    u64 a = 5, b = 2, c = 5;
    if (xcCompare_u64(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_u64(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_u64(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_u64(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_isize(void) {
    isize a = 5, b = 2, c = 5;
    if (xcCompare_isize(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_isize(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_isize(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_isize(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_usize(void) {
    usize a = 5, b = 2, c = 5;
    if (xcCompare_usize(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_usize(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_usize(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_usize(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_f32(void) {
    f32 a = 5.0f, b = 2.0f, c = 5.0f;
    if (xcCompare_f32(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_f32(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_f32(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_f32(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

TestResult test_xcCompare_f64(void) {
    f64 a = 5.0, b = 2.0, c = 5.0;
    if (xcCompare_f64(&a, &b) <= 0)
        return TR_failure;
    if (xcCompare_f64(&b, &a) >= 0)
        return TR_failure;
    if (xcCompare_f64(&a, &c) != 0)
        return TR_failure;
    if (xcCompare_f64(&c, &a) != 0)
        return TR_failure;
    return TR_success;
}

void test_xcCompare__addTests(void) {
    testSetModule("test_xcCompare");
    testAdd(TEST(test_xcCompare_int));
    testAdd(TEST(test_xcCompare_uint));
    testAdd(TEST(test_xcCompare_i8));
    testAdd(TEST(test_xcCompare_u8));
    testAdd(TEST(test_xcCompare_i16));
    testAdd(TEST(test_xcCompare_u16));
    testAdd(TEST(test_xcCompare_i32));
    testAdd(TEST(test_xcCompare_u32));
    testAdd(TEST(test_xcCompare_i64));
    testAdd(TEST(test_xcCompare_u64));
    testAdd(TEST(test_xcCompare_isize));
    testAdd(TEST(test_xcCompare_usize));
    testAdd(TEST(test_xcCompare_f32));
    testAdd(TEST(test_xcCompare_f64));
}
