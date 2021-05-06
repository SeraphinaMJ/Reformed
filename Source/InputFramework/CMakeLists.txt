cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(InputFramework STATIC
    Public/InputFramework.hpp Private/InputFramework.cpp
    Public/GamepadControlDefines.hpp
	Public/InputConstants.hpp
)

# expose headers
target_include_directories(InputFramework
    PUBLIC Public
)

# link to dependencies
target_link_libraries(InputFramework
    PRIVATE EngineInterfaces
	PRIVATE ComponentCore
	PRIVATE Logger
)
