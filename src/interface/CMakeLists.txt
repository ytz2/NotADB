project(cpp_interface)

# name the lib as interface and use INTERFACE to not allow any .cc file
add_library(interface INTERFACE)

# alias the interface
add_library(sub::interface ALIAS interface)

# specify the include path
#target_include_directories(interface INTERFACE interface/)
#usage target_link_library(another-users-target interface)