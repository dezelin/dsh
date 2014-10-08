cmake_minimum_required(VERSION 2.8.4)
project(dsh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(dsh ${SOURCE_FILES})
