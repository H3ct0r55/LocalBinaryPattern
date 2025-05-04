# x86_64-linux-toolchain.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Prevent macOS interference
set(CMAKE_OSX_ARCHITECTURES "")
set(CMAKE_OSX_SYSROOT "")

# Compilers
set(CMAKE_C_COMPILER /opt/homebrew/bin/x86_64-unknown-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/x86_64-unknown-linux-gnu-g++)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Static linking and optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11 -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--gc-sections")