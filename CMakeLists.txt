cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp)  

target_link_libraries(untitled user32 gdi32)
