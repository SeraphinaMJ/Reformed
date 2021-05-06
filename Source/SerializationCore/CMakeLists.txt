cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(SerializationCore STATIC
    Public/Reflection.hpp Private/Reflection.cpp
)

# expose headers
target_include_directories(SerializationCore
    PUBLIC Public
)

# link to dependencies
target_link_libraries(SerializationCore
    PRIVATE Logger
    PUBLIC Utility
)

set_target_properties(SerializationCore PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

