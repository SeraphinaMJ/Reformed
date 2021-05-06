cmake_minimum_required(VERSION 3.15.0)

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)

if(WIN32)
    find_package(GLEWInternal REQUIRED)
else()
    find_package(GLEW REQUIRED)
    find_package(LinuxWinDefs REQUIRED)
endif()

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H -fsyntax-only")

add_library(Graphics.OpenGL4 STATIC
    DLLControl.hpp
    #dummy.cpp # <-- TODO: Remove this
    GLFWAdapter.hpp GLFWAdapter.cpp
    GlfwWindow.hpp GlfwWindow.cpp
    GlfwWindowData.hpp

    )

target_include_directories(Graphics.OpenGL4
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${OPENGL_INCLUDE_DIRS}
    PRIVATE GLEWInternal
    PUBLIC GLFW
    PRIVATE ${LinuxWinDefs_INCLUDES} # TODO: Will this work even on Windows?

    # TODO: Fix these references \/ \/ \/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/TempestEngine
)

target_link_libraries(Graphics.OpenGL4
    PUBLIC EngineInterfaces
    PUBLIC GLFW
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE GLEWInternal
    PRIVATE Utility
    PRIVATE EventSystem
    )

# build up resources list
list(APPEND resources ${GLEWInternal_RESOURCES})
list(APPEND resources ${GLFW_RESOURCES})
set(Graphics.OpenGL4_RESOURCES ${resources} CACHE STRING "Graphics.OpenGL4_RESOURCES" FORCE)

set_target_properties(Graphics.OpenGL4 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)
