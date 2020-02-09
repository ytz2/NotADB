# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("connectivity")
#set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_BUILD_TYPE Debug)
# thus include <interface/xxx.h> will work
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) 
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
# Include sub-projects.
add_subdirectory ("interface")
add_subdirectory ("lib")
add_subdirectory ("app")

