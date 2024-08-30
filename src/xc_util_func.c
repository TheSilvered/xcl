#include "xc.h"

XCComparisonResult xcCompare_int(int *a, int *b) {
    int va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_uint(uint *a, uint *b) {
    uint va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_i8(i8 *a, i8 *b) {
    i8 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_u8(u8 *a, u8 *b) {
    u8 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_i16(i16 *a, i16 *b) {
    i16 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_u16(u16 *a, u16 *b) {
    u16 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_i32(i32 *a, i32 *b) {
    i32 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_u32(u32 *a, u32 *b) {
    u32 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_i64(i64 *a, i64 *b) {
    i64 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_u64(u64 *a, u64 *b) {
    u64 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_isize(isize *a, isize *b) {
    isize va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_usize(usize *a, usize *b) {
    usize va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_f32(f32 *a, f32 *b) {
    f32 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}

XCComparisonResult xcCompare_f64(f64 *a, f64 *b) {
    f64 va = *a, vb = *b;
    if (va == vb)
        return XC_CR_EQUAL;
    else if (va < vb)
        return XC_CR_LESS_THAN;
    else
        return XC_CR_GREATER_THAN;
}
