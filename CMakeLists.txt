cmake_minimum_required(VERSION 3.24)
project(calculator)

set(CMAKE_CXX_STANDARD 20)

add_executable(calculator main.cpp IOControl.cpp IOControl.h Calculator.cpp Calculator.h)
add_subdirectory(tests)