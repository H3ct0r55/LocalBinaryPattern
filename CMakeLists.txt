cmake_minimum_required(VERSION 3.29)
project(LocalBinaryPattern)

set(CMAKE_CXX_STANDARD 20)


add_executable(LocalBinaryPattern main.cpp
        Image.cpp
        Image.h
        help_text.h)

# Ensure maximum stripping of symbols from the final binary
set(STRIP_EXECUTABLE /opt/homebrew/bin/aarch64-unknown-linux-gnu-strip)
add_custom_command(TARGET LocalBinaryPattern POST_BUILD
        COMMAND ${STRIP_EXECUTABLE} $<TARGET_FILE:LocalBinaryPattern>
)
