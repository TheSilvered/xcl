#!/usr/bin/sh

clang -o test_main test_main.c ./test_lib/test.c ../src/* -I../include -I./test_lib -DXC_DEBUG_MEM -DTEST_NO_THREADS -g -O0 && ./test_main $@
