find_package(ImGui REQUIRED)
find_package(GLFW REQUIRED)

if(WIN32)
    find_package(GLEWInternal REQUIRED)
else()
    find_package(GLEW REQUIRED)
endif()

find_path(ImGui_ROOT
    NAMES imgui.h
    PATH_SUFFIXES imgui-docking
)

find_path(ImGui_OpenGL3_GLFW_GLEW_INCLUDE_DIRS
    NAMES imgui_impl_opengl3.h
    PATHS ${ImGui_ROOT}
    PATH_SUFFIXES examples
)

find_file(ImGui_Impl_OpenGL3_SOURCES
    imgui_impl_opengl3.cpp
    PATHS ${ImGui_OpenGL3_GLFW_GLEW_INCLUDE_DIRS}
)
find_file(ImGui_Impl_GLFW_SOURCES
    imgui_impl_glfw.cpp
    PATHS ${ImGui_OpenGL3_GLFW_GLEW_INCLUDE_DIRS}
)
set(ImGui_OpenGL3_GLFW_GLEW_SOURCES
    ${ImGui_Impl_OpenGL3_SOURCES}
    ${ImGui_Impl_GLFW_SOURCES}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ImGui_OpenGL3_GLFW_GLEW
    FOUND_VAR
        ImGui_OpenGL3_GLFW_GLEW_FOUND
    REQUIRED_VARS
        ImGui_OpenGL3_GLFW_GLEW_INCLUDE_DIRS
        ImGui_OpenGL3_GLFW_GLEW_SOURCES
)

if(ImGui_OpenGL3_GLFW_GLEW_FOUND)
    if(NOT TARGET ImGui_OpenGL3_GLFW_GLEW)
        add_library(ImGui_OpenGL3_GLFW_GLEW
            ${ImGui_OpenGL3_GLFW_GLEW_SOURCES})
        target_include_directories(ImGui_OpenGL3_GLFW_GLEW
            SYSTEM PUBLIC ${ImGui_OpenGL3_GLFW_GLEW_INCLUDE_DIRS})
        target_link_libraries(ImGui_OpenGL3_GLFW_GLEW
            PUBLIC ImGui
            PUBLIC GLFW
            PUBLIC GLEWInternal)
        target_compile_definitions(ImGui_OpenGL3_GLFW_GLEW
            PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)
    endif(NOT TARGET ImGui_OpenGL3_GLFW_GLEW)
endif(ImGui_OpenGL3_GLFW_GLEW_FOUND)
