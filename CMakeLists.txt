set(CMAKE_TOOLCHAIN_FILE android-cmake/android.toolchain.cmake)
set(ANDROID_NDK $ENV{ANDROID_NDK})

set(ANDROID_NO_UNDEFINED OFF)
set(ANDROID_NATIVE_API_LEVEL "android-19")
set(ANDROID_TOOLCHAIN_NAME "arm-linux-androideabi-4.9")
set(ANDROID_ABI "armeabi-v7a")

project(HORIZON)
cmake_minimum_required(VERSION 2.8)

include_directories(include)
file(GLOB SOURCES src/*)
add_executable(horizon ${SOURCES})

file(GLOB MODULES modules/*.c)
foreach (module ${MODULES})
  get_filename_component(result ${module} NAME_WE)
  add_library(${result} SHARED ${module})
  set_target_properties(${result} PROPERTIES PREFIX "")
  set_target_properties(${result} PROPERTIES SUFFIX ".module")
endforeach(module)
