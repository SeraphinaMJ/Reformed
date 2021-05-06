cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(Configuration STATIC
    Private/HEADER_ONLY_MODULE.cpp
    Public/Configuration.hpp
)

# expose headers
target_include_directories(Configuration
    PUBLIC Public
)

# link to dependencies
#target_link_libraries(Configuration
#    PRIVATE <dependencyModule>
#)

set_target_properties(Configuration PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)