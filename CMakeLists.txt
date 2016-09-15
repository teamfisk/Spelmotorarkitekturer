cmake_minimum_required(VERSION 3.6)
project(Spelmotorarkitekturer_Minnesallokator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
    Allocator/Main.cpp
    Allocator/StackAllocator.h
)

add_executable(Spelmotorarkitekturer_Minnesallokator ${SOURCE_FILES})