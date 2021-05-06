cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(EngineInterfaces STATIC
    Private/HEADER_ONLY_MODULE.cpp
    Public/IComponentMemoryClasses.hpp
)

# expose headers
target_include_directories(EngineInterfaces
    PUBLIC Public
)

# link to dependencies
#target_link_libraries(EngineInterfaces
#    PRIVATE <dependencyModule>
#)