cmake_minimum_required(VERSION 3.2)
project(exeModel)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)
set(CMAKE_C_COMPILER "/opt/compiler/gcc-4.8.2/bin/gcc")
set(CMAKE_CXX_COMPILER "/opt/compiler/gcc-4.8.2/bin/g++")

message(status ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/mkl)
set(SOURCE_FILES main.cpp common.h modelInput.h modelInput.cpp)

link_directories(${CMAKE_SOURCE_DIR}/mkl/lib)

add_executable(exeModel ${SOURCE_FILES})
target_link_libraries (exeModel Threads::Threads  paddle_capi_shared)
