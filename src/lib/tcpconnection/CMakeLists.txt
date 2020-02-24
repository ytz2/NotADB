cmake_minimum_required(VERSION 3.8)
project(libtcpconnectoin)

#external lib
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

find_package(jsoncpp REQUIRED)
find_package(GLog REQUIRED)
INCLUDE_DIRECTORIES( jsoncpp )

add_library(libtcpconnectoin MessageCodec.cpp TcpConnection.cpp)
# alias the lib
add_library(sub::libtcpconnectoin ALIAS libtcpconnectoin)

# Specify that this library depends upon below
target_link_libraries(libtcpconnectoin  
	interface
	sub::libmessage
	${USED_LIBS}
	jsoncpp
	${GLOG_LIBRARIES}
)

