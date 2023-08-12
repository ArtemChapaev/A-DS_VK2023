cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(1task)

project(2task)
add_executable(2task 2task.cpp)

project(3task)
add_executable(3task 3task.cpp)

project(4task)
add_executable(4task 4task.cpp)
