# aarch64-linux-toolchain.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Prevent CMake from using macOS-specific flags
set(CMAKE_OSX_ARCHITECTURES "")
set(CMAKE_OSX_SYSROOT "")

# Point to the cross-compilers
set(CMAKE_C_COMPILER /opt/homebrew/bin/aarch64-unknown-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/aarch64-unknown-linux-gnu-g++)

# Optional: Set sysroot if you have one (e.g., from a Linux image)
# set(CMAKE_SYSROOT /path/to/aarch64-linux-sysroot)

# Force static linking of stdlibc++

# Default to Release build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optional optimizations for smaller statically-linked builds
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11 -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--gc-sections")