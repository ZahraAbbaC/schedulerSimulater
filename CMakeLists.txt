cmake_minimum_required(VERSION 3.9)
project(project1 C)

set(CMAKE_C_STANDARD 99)

add_executable(project1 main.c list.c list.h)