cmake_minimum_required(VERSION 3.8)
project(SimpleTCPServer)

find_package (Threads)
find_package(GLog REQUIRED)
# Create the executable
add_executable(${PROJECT_NAME} main.cpp)
find_package(GLog REQUIRED)
# Link the static library from subproject1 using it's alias sub::lib1
# Link the header only library from subproject2 using it's alias sub::lib2
# This will cause the include directories for that target to be added to this project
target_link_libraries(${PROJECT_NAME}
	sub::libmessage
    sub::libtcpconnectoin
    sub::interface
    ${CMAKE_THREAD_LIBS_INIT}
    ${GLOG_LIBRARIES}
)