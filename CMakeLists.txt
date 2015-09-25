cmake_minimum_required(VERSION 3.3)
project(osproj1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES threaded.cpp ThreadPool.cpp ThreadPool.h)
add_executable(osproj1 ${SOURCE_FILES})