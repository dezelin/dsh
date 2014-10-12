cmake_minimum_required(VERSION 2.8.4)
project(dsh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(Boost 1.54 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
set(SOURCE_FILES src/main.cpp)
add_executable(dsh ${SOURCE_FILES})
target_link_libraries(dsh ${Boost_LIBRARIES})
