#!/usr/bin/sh

CC=clang
EXE_OUT=main
SRCS="main.c src/*"
INCLUDE="-I./include"
DEFINES=
FLAGS="-g -O0"

$CC -o $EXE_OUT $SRCS $INCLUDE $DEFINES $FLAGS && ./$EXE_OUT $@
