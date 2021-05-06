find_package(GLFW REQUIRED)

find_path(ImGui_ROOT
    NAMES imgui.h
    PATH_SUFFIXES imgui-docking
)

find_path(ImGui_INCLUDE_DIRS_core
    NAMES imgui.h
    PATHS ${ImGui_ROOT}
)
find_path(ImGui_INCLUDE_DIRS_misc
    NAMES imgui_stdlib.h
    PATHS ${ImGui_ROOT}
    PATH_SUFFIXES misc/cpp
)
set(ImGui_INCLUDE_DIRS
    ${ImGui_INCLUDE_DIRS_core}
    ${ImGui_INCLUDE_DIRS_misc}
)

find_file(ImGui_SOURCES_imgui
    imgui.cpp
    PATHS ${ImGui_ROOT}
)
find_file(ImGui_SOURCES_imgui_draw
    imgui_draw.cpp
    PATHS ${ImGui_ROOT}
)
find_file(ImGui_SOURCES_imgui_widgets
    imgui_widgets.cpp
    PATHS ${ImGui_ROOT}
)
find_file(ImGui_SOURCES_imgui_demo
    imgui_demo.cpp
    PATHS ${ImGui_ROOT}
)
set(ImGui_SOURCES
    ${ImGui_SOURCES_imgui}
    ${ImGui_SOURCES_imgui_draw}
    ${ImGui_SOURCES_imgui_widgets}
    ${ImGui_SOURCES_imgui_demo}
)

find_file(ImGui_Stdlib_SOURCES
    imgui_stdlib.cpp
    PATHS ${ImGui_ROOT}
    PATH_SUFFIXES misc/cpp
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ImGui
    FOUND_VAR
        ImGui_FOUND
    REQUIRED_VARS
        ImGui_INCLUDE_DIRS
        ImGui_SOURCES
        ImGui_Stdlib_SOURCES
)

if(ImGui_FOUND)
    if(NOT TARGET ImGui)
        add_library(ImGui ${ImGui_SOURCES})
        target_include_directories(ImGui SYSTEM PUBLIC ${ImGui_INCLUDE_DIRS})
    endif(NOT TARGET ImGui)
    if(NOT TARGET ImGui_Stdlib)
        add_library(ImGui_Stdlib ${ImGui_Stdlib_SOURCES})
        target_include_directories(ImGui_Stdlib SYSTEM PUBLIC ${ImGui_INCLUDE_DIRS})
        target_link_libraries(ImGui_Stdlib PUBLIC ImGui)
    endif(NOT TARGET ImGui_Stdlib)
endif(ImGui_FOUND)
