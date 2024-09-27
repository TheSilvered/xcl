#!/usr/bin/sh

CC=clang
EXE_OUT=test_main
SRCS="test_main.c ./test_lib/test.c ../src/*"
INCLUDE="-I../include -I./test_lib"
DEFINES="-DXC_DEBUG_MEM"
FLAGS="-g -O0"

$CC -o $EXE_OUT $SRCS $INCLUDE $DEFINES $FLAGS && ./$EXE_OUT $@
