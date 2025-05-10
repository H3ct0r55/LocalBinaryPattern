cmake_minimum_required(VERSION 3.29)
project(LocalBinaryPattern)

set(CMAKE_CXX_STANDARD 20)


add_executable(LocalBinaryPattern main.cpp
        Image.cpp
        Image.h
        help_text.h
        InteractiveWizard.h
        InteractiveWizard.cpp
        TypeDetect.h
        TypeDetect.cpp
)

# Ensure maximum stripping of symbols from the final binary
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(STRIP_EXECUTABLE /opt/homebrew/bin/aarch64-unknown-linux-gnu-strip)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(STRIP_EXECUTABLE /opt/homebrew/bin/x86_64-unknown-linux-gnu-strip)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(STRIP_EXECUTABLE /opt/homebrew/bin/x86_64-w64-mingw32-strip)
    add_custom_command(TARGET LocalBinaryPattern POST_BUILD
            COMMAND ${STRIP_EXECUTABLE} $<TARGET_FILE:LocalBinaryPattern>
    )
endif()

add_custom_command(TARGET LocalBinaryPattern POST_BUILD
    COMMAND ${STRIP_EXECUTABLE} $<TARGET_FILE:LocalBinaryPattern>
)
