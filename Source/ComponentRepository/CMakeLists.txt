cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(ComponentRepository STATIC
    Public/ComponentRepository.hpp Private/ComponentRepository.cpp
)

# expose headers
target_include_directories(ComponentRepository
    PUBLIC Public
)

# link to dependencies
target_link_libraries(ComponentRepository
    PUBLIC ComponentCore
    PRIVATE EngineInterfaces
    PRIVATE SerializationCore
    PRIVATE Logger
)

set_target_properties(ComponentRepository PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)