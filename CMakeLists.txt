cmake_minimum_required(VERSION 3.5)
project(onion_buse)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c buse.c)
add_executable(onion_buse ${SOURCE_FILES})
