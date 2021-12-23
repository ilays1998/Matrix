cmake_minimum_required(VERSION 3.20)
project(ex3_ilaysoffer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex3_ilaysoffer
        ex3.c
        ex3.h main.c test_runner.c tests.c)
