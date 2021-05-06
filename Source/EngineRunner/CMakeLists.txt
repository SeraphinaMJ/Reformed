cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(EngineRunner STATIC
    Public/EngineRunner.hpp Private/EngineRunner.cpp
)

# expose headers
target_include_directories(EngineRunner
    PUBLIC Public
)

# link to dependencies
target_link_libraries(EngineRunner
    PUBLIC Configuration
    PRIVATE EngineInterfaces
    PRIVATE ComponentRepository
    PRIVATE TempestEngine
    PRIVATE EventSystem
    PRIVATE Math
)

set_target_properties(EngineRunner PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)
