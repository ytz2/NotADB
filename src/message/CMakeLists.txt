cmake_minimum_required(VERSION 3.8)
project(libmessage)

find_package(jsoncpp REQUIRED)
INCLUDE_DIRECTORIES( jsoncpp )

add_library(libmessage MessageFactory.cpp MessageFactory.h commonjson/CommonJsonFactory.h commonjson/CommonJsonMessage.h)
# alias the lib
add_library(sub::libmessage ALIAS libmessage)

# Specify that this library depends upon below
target_link_libraries(libmessage  
	jsoncpp
	sub::interface
)

