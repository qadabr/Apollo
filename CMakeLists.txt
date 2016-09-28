set(CMAKE_TOOLCHAIN_FILE android-cmake/android.toolchain.cmake)
set(ANDROID_NDK $ENV{ANDROID_NDK})

set(ANDROID_NATIVE_API_LEVEL "android-19")
set(ANDROID_TOOLCHAIN_NAME "arm-linux-androideabi-4.9")
set(ANDROID_ABI "armeabi-v7a")

project(HORIZON)
cmake_minimum_required(VERSION 2.8)

include_directories(include)
add_executable(horizon src/main.cpp src/Module.cpp)
