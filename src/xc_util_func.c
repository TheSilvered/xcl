#include "xc.h"

XCLIB int xcCompare_int(XCRef a, XCRef b) {
    int va = *(int *)a;
    int vb = *(int *)b;
    return va - vb;
}

XCLIB int xcCompare_uint(XCRef a, XCRef b) {
    uint va = *(uint *)a;
    uint vb = *(uint *)b;
    return va - vb;
}

XCLIB int xcCompare_i8(XCRef a, XCRef b) {
    i8 va = *(i8 *)a;
    i8 vb = *(i8 *)b;
    return va - vb;
}

XCLIB int xcCompare_u8(XCRef a, XCRef b) {
    u8 va = *(u8 *)a;
    u8 vb = *(u8 *)b;
    return va - vb;
}

XCLIB int xcCompare_i16(XCRef a, XCRef b) {
    i16 va = *(i16 *)a;
    i16 vb = *(i16 *)b;
    return va - vb;
}

XCLIB int xcCompare_u16(XCRef a, XCRef b) {
    u16 va = *(u16 *)a;
    u16 vb = *(u16 *)b;
    return va - vb;
}

XCLIB int xcCompare_i32(XCRef a, XCRef b) {
    i32 va = *(i32 *)a;
    i32 vb = *(i32 *)b;
    return va - vb;
}

XCLIB int xcCompare_u32(XCRef a, XCRef b) {
    u32 va = *(u32 *)a;
    u32 vb = *(u32 *)b;
    return va - vb;
}

XCLIB int xcCompare_i64(XCRef a, XCRef b) {
    i64 va = *(i64 *)a;
    i64 vb = *(i64 *)b;
    return va - vb;
}

XCLIB int xcCompare_u64(XCRef a, XCRef b) {
    u64 va = *(u64 *)a;
    u64 vb = *(u64 *)b;
    return va - vb;
}

XCLIB int xcCompare_isize(XCRef a, XCRef b) {
    isize va = *(isize *)a;
    isize vb = *(isize *)b;
    return va - vb;
}

XCLIB int xcCompare_usize(XCRef a, XCRef b) {
    usize va = *(usize *)a;
    usize vb = *(usize *)b;
    return va - vb;
}

XCLIB int xcCompare_f32(XCRef a, XCRef b) {
    f32 va = *(f32 *)a;
    f32 vb = *(f32 *)b;
    if (va < vb)
        return -1;
    else if (va > vb)
        return 1;
    return 0;
}

XCLIB int xcCompare_f64(XCRef a, XCRef b) {
    f64 va = *(f64 *)a;
    f64 vb = *(f64 *)b;
    if (va < vb)
        return -1;
    else if (va > vb)
        return 1;
    return 0;
}

XCLIB int xcMin_int(int a, int b) {
    return b < a ? b : a;
}

XCLIB uint xcMin_uint(uint a, uint b) {
    return b < a ? b : a;
}

XCLIB i8 xcMin_i8(i8 a, i8 b) {
    return b < a ? b : a;
}

XCLIB u8 xcMin_u8(u8 a, u8 b) {
    return b < a ? b : a;
}

XCLIB i16 xcMin_i16(i16 a, i16 b) {
    return b < a ? b : a;
}

XCLIB u16 xcMin_u16(u16 a, u16 b) {
    return b < a ? b : a;
}

XCLIB i32 xcMin_i32(i32 a, i32 b) {
    return b < a ? b : a;
}

XCLIB u32 xcMin_u32(u32 a, u32 b) {
    return b < a ? b : a;
}

XCLIB i64 xcMin_i64(i64 a, i64 b) {
    return b < a ? b : a;
}

XCLIB u64 xcMin_u64(u64 a, u64 b) {
    return b < a ? b : a;
}

XCLIB isize xcMin_isize(isize a, isize b) {
    return b < a ? b : a;
}

XCLIB usize xcMin_usize(usize a, usize b) {
    return b < a ? b : a;
}

XCLIB f32 xcMin_f32(f32 a, f32 b) {
    return b < a ? b : a;
}

XCLIB f64 xcMin_f64(f64 a, f64 b) {
    return b < a ? b : a;
}

XCLIB XCRef xcMin(XCRef a, XCRef b, XCComparator compareFunc) {
    return compareFunc(b, a) < 0 ? b : a;
}

XCLIB int xcMax_int(int a, int b) {
    return b > a ? b : a;
}

XCLIB uint xcMax_uint(uint a, uint b) {
    return b > a ? b : a;
}

XCLIB i8 xcMax_i8(i8 a, i8 b) {
    return b > a ? b : a;
}

XCLIB u8 xcMax_u8(u8 a, u8 b) {
    return b > a ? b : a;
}

XCLIB i16 xcMax_i16(i16 a, i16 b) {
    return b > a ? b : a;
}

XCLIB u16 xcMax_u16(u16 a, u16 b) {
    return b > a ? b : a;
}

XCLIB i32 xcMax_i32(i32 a, i32 b) {
    return b > a ? b : a;
}

XCLIB u32 xcMax_u32(u32 a, u32 b) {
    return b > a ? b : a;
}

XCLIB i64 xcMax_i64(i64 a, i64 b) {
    return b > a ? b : a;
}

XCLIB u64 xcMax_u64(u64 a, u64 b) {
    return b > a ? b : a;
}

XCLIB isize xcMax_isize(isize a, isize b) {
    return b > a ? b : a;
}

XCLIB usize xcMax_usize(usize a, usize b) {
    return b > a ? b : a;
}

XCLIB f32 xcMax_f32(f32 a, f32 b) {
    return b > a ? b : a;
}

XCLIB f64 xcMax_f64(f64 a, f64 b) {
    return b > a ? b : a;
}

XCLIB XCRef xcMax(XCRef a, XCRef b, XCComparator compareFunc) {
    return compareFunc(b, a) > 0 ? b : a;
}

XCLIB u32 xcHash_int(XCRef num) {
    return (u32)((*(int *)num) & 0xFFFFFFFF);
}

XCLIB u32 xcHash_uint(XCRef num) {
    return (u32)((*(uint *)num) & 0xFFFFFFFF);
}

XCLIB u32 xcHash_i8(XCRef num) {
    return (u32)(*(i8 *)num);
}

XCLIB u32 xcHash_u8(XCRef num) {
    return (u32)(*(u8 *)num);
}

XCLIB u32 xcHash_i16(XCRef num) {
    return (u32)(*(i16 *)num);
}

XCLIB u32 xcHash_u16(XCRef num) {
    return (u32)(*(u16 *)num);
}

XCLIB u32 xcHash_i32(XCRef num) {
    return (u32)(*(i32 *)num);
}

XCLIB u32 xcHash_u32(XCRef num) {
    return *(u32 *)num;
}

XCLIB u32 xcHash_i64(XCRef num) {
    return (u32)((*(i64 *)num) & 0xFFFFFFFF);
}

XCLIB u32 xcHash_u64(XCRef num) {
    return (u32)((*(u64 *)num) & 0xFFFFFFFF);
}

XCLIB u32 xcHash_isize(XCRef num) {
    return (u32)((*(isize *)num) & 0xFFFFFFFF);
}

XCLIB u32 xcHash_usize(XCRef num) {
    return (u32)((*(usize *)num) & 0xFFFFFFFF);
}
