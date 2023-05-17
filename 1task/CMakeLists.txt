cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(1task)
add_executable(1task /
        ListGraph.cpp /
        MatrixGraph.cpp /
        SetGraph.cpp /
        ArcGraph.cpp /
        main.cpp /
        )
