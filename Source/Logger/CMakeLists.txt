cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(Logger STATIC
    Public/Logger.hpp Private/Logger.cpp
)

# expose headers
target_include_directories(Logger
    PUBLIC Public
)

# link to dependencies
#target_link_libraries(Logger
#)

