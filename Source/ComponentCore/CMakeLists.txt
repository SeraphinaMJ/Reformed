cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(ComponentCore STATIC
    Public/Component.hpp Private/Component.cpp
    Public/ComponentType.hpp
    Public/ComponentTypeDetails.hpp
    Public/ComponentRegistrar.hpp
)

# expose headers
target_include_directories(ComponentCore
    PUBLIC Public
)

# link to dependencies
target_link_libraries(ComponentCore
    PRIVATE SerializationCore
    PUBLIC EngineInterfaces
)

