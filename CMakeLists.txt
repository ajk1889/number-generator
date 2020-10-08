cmake_minimum_required(VERSION 3.16)
project(generator)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 17)

add_executable(generator main.cpp Generator.h Generator.cpp Pair.h)