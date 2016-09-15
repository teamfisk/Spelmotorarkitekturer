cmake_minimum_required(VERSION 3.6)
project(Spelmotorarkitekturer_Minnesallokator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(Allocator)
add_subdirectory(AllocatorTest)