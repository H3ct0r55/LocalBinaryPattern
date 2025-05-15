cmake_minimum_required(VERSION 3.29)
project(LocalBinaryPattern)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")


add_executable(LocalBinaryPattern src/main.cpp
        src/Image/Image.cpp
        src/Image/Image.h
        src/HelpText/help_text.h
        src/InteractiveWizard/InteractiveWizard.h
        src/InteractiveWizard/InteractiveWizard.cpp
        src/TypeDetect/TypeDetect.h
        src/TypeDetect/TypeDetect.cpp
        src/Image/Image.cpp
)

#
# Ensure maximum stripping of symbols from the final binary
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(STRIP_EXECUTABLE /opt/homebrew/bin/aarch64-unknown-linux-gnu-strip)
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(STRIP_EXECUTABLE /opt/homebrew/bin/x86_64-unknown-linux-gnu-strip)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(STRIP_EXECUTABLE /opt/homebrew/bin/x86_64-w64-mingw32-strip)
    endif()
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_custom_command(TARGET LocalBinaryPattern POST_BUILD
        COMMAND ${STRIP_EXECUTABLE} $<TARGET_FILE:LocalBinaryPattern>
    )
endif()
