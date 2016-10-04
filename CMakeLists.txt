set(PROJECT_NAME eucalyptus)

set(CMAKE_TOOLCHAIN_FILE android-cmake/android.toolchain.cmake)
set(ANDROID_NDK $ENV{ANDROID_NDK})

set(ANDROID_NATIVE_API_LEVEL "android-19")
set(ANDROID_TOOLCHAIN_NAME "arm-linux-androideabi-4.8")
set(ANDROID_ABI "armeabi-v7a")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.8)

set(LIBRARY_DEPS log)

include_directories(include)
file(GLOB SOURCES src/*)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_DEPS})

file(GLOB MODULES modules/*)
foreach (module_src ${MODULES})
  get_filename_component(module ${module_src} NAME_WE)
  string(TOLOWER ${module} module)
  add_library(${module} SHARED ${module_src})
  set_target_properties(${module} PROPERTIES PREFIX "")
  set_target_properties(${module} PROPERTIES SUFFIX ".em")
  target_link_libraries(${module} ${LIBRARY_DEPS})
endforeach(module_src)
