cmake_minimum_required(VERSION 3.29)
project(LocalBinaryPattern)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(LocalBinaryPattern main.cpp
        Image.cpp
        Image.h)
