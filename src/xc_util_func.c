#include "xc.h"

XCLIB int xcCompare_int(void *a, void *b) {
    int va = *(int *)a;
    int vb = *(int *)b;
    return va - vb;
}

XCLIB int xcCompare_uint(void *a, void *b) {
    uint va = *(uint *)a;
    uint vb = *(uint *)b;
    return va - vb;
}

XCLIB int xcCompare_i8(void *a, void *b) {
    i8 va = *(i8 *)a;
    i8 vb = *(i8 *)b;
    return va - vb;
}

XCLIB int xcCompare_u8(void *a, void *b) {
    u8 va = *(u8 *)a;
    u8 vb = *(u8 *)b;
    return va - vb;
}

XCLIB int xcCompare_i16(void *a, void *b) {
    i16 va = *(i16 *)a;
    i16 vb = *(i16 *)b;
    return va - vb;
}

XCLIB int xcCompare_u16(void *a, void *b) {
    u16 va = *(u16 *)a;
    u16 vb = *(u16 *)b;
    return va - vb;
}

XCLIB int xcCompare_i32(void *a, void *b) {
    i32 va = *(i32 *)a;
    i32 vb = *(i32 *)b;
    return va - vb;
}

XCLIB int xcCompare_u32(void *a, void *b) {
    u32 va = *(u32 *)a;
    u32 vb = *(u32 *)b;
    return va - vb;
}

XCLIB int xcCompare_i64(void *a, void *b) {
    i64 va = *(i64 *)a;
    i64 vb = *(i64 *)b;
    return va - vb;
}

XCLIB int xcCompare_u64(void *a, void *b) {
    u64 va = *(u64 *)a;
    u64 vb = *(u64 *)b;
    return va - vb;
}

XCLIB int xcCompare_isize(void *a, void *b) {
    isize va = *(isize *)a;
    isize vb = *(isize *)b;
    return va - vb;
}

XCLIB int xcCompare_usize(void *a, void *b) {
    usize va = *(usize *)a;
    usize vb = *(usize *)b;
    return va - vb;
}

XCLIB int xcCompare_f32(void *a, void *b) {
    f32 va = *(f32 *)a;
    f32 vb = *(f32 *)b;
    if (va < vb)
        return -1;
    else if (va > vb)
        return 1;
    return 0;
}

XCLIB int xcCompare_f64(void *a, void *b) {
    f64 va = *(f64 *)a;
    f64 vb = *(f64 *)b;
    if (va < vb)
        return -1;
    else if (va > vb)
        return 1;
    return 0;
}
