cmake_minimum_required(VERSION 3.15)
project(vector_search_engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE include)
