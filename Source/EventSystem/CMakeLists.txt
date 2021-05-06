cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(EventSystem STATIC
    Private/EventBus.cpp
    Private/EventBase.cpp
    Private/EventFireError.cpp
    Private/EventHandler.cpp
)

# expose headers
target_include_directories(EventSystem
    PUBLIC Public
)

# link to dependencies
target_link_libraries(EventSystem
    PRIVATE Utility
    PRIVATE Logger
)


