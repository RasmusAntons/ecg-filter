cmake_minimum_required(VERSION 3.2)
project(filter)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c)
add_executable(filter ${SOURCE_FILES} filters.h filters.c csv.h csv.c graph.h graph.c)