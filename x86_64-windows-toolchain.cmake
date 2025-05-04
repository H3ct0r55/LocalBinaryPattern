# x86_64-windows-toolchain.cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Prevent macOS interference
set(CMAKE_OSX_ARCHITECTURES "")
set(CMAKE_OSX_SYSROOT "")

set(CMAKE_C_COMPILER /opt/homebrew/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/x86_64-w64-mingw32-g++)

# Optional: static linking
set(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--gc-sections")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++17 -ffunction-sections -fdata-sections")

# Release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()