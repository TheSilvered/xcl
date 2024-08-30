cmake_minimum_required(VERSION 3.13.0)
project(arrayTest VERSION 0.1.0 LANGUAGES C)

add_library(xc "src/xc_array.c" "src/xc_mem.c" "src/xc_util_func.c")
target_include_directories(xc PUBLIC include)

add_executable(arrayTest main.c)
target_include_directories(arrayTest PUBLIC include)
target_link_libraries(arrayTest xc)
