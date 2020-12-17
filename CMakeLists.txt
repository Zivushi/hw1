cmake_minimum_required(VERSION 3.17)
project(newmtm C)

set(CMAKE_C_STANDARD 99)

add_library(newmtm priority_queue.c priority_queue.h)