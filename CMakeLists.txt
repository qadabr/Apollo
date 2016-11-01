set(PROJECT_NAME apollo)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_TOOLCHAIN_FILE android-cmake/android.toolchain.cmake)
set(ANDROID_NDK $ENV{ANDROID_NDK})

set(ANDROID_NATIVE_API_LEVEL "android-19")
set(ANDROID_TOOLCHAIN_NAME "arm-linux-androideabi-4.8")
set(ANDROID_ABI "armeabi-v7a")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 3.1)

add_subdirectory(aquila-src)
include_directories(aquila-src)

set(LIBRARY_DEPS log OpenSLES Aquila)

include_directories(include)
file(GLOB SOURCES source/*.c*)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

#file(GLOB MODULES modules/*/*.cpp)
foreach (module_src ${MODULES})
  get_filename_component(module ${module_src} NAME_WE)
  string(TOLOWER ${module} module)
  add_library(${module} SHARED ${module_src})
  set_target_properties(${module} PROPERTIES PREFIX "")
  set_target_properties(${module} PROPERTIES SUFFIX ".m")
  target_link_libraries(${module} ${LIBRARY_DEPS})
endforeach(module_src)
