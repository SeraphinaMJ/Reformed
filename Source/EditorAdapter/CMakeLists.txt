cmake_minimum_required(VERSION 3.15.0)

find_package(GLFW REQUIRED)

# define target
add_library(EditorAdapter STATIC
   Public/GLFWEditorAdapter.hpp Private/GLFWEditorAdapter.cpp
)
   
# expose headers
target_include_directories(EditorAdapter
   PUBLIC Public
)
   
# link to dependencies
target_link_libraries(EditorAdapter
    PUBLIC Graphics.OpenGL4
	PRIVATE GLFW
)
