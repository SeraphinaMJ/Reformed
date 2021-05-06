find_package(ImGui REQUIRED)

find_path(ImGuizmo_ROOT
    NAMES ImGuizmo.h
    PATH_SUFFIXES ImGuizmo-master
)

find_path(ImGuizmo_INCLUDE_DIRS
    NAMES ImGuizmo.h
    PATHS ${ImGuizmo_ROOT}
)

find_file(ImGuizmo_SOURCES
    ImGuizmo.cpp
    PATHS ${ImGuizmo_ROOT}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ImGuizmo
    FOUND_VAR
        ImGuizmo_FOUND
    REQUIRED_VARS
        ImGuizmo_INCLUDE_DIRS
        ImGuizmo_SOURCES
)

if(ImGuizmo_FOUND)
    if(NOT TARGET ImGuizmo)
        add_library(ImGuizmo ${ImGuizmo_SOURCES})
        target_include_directories(ImGuizmo SYSTEM PUBLIC ${ImGuizmo_INCLUDE_DIRS}
            SYSTEM PUBLIC ${ImGui_INCLUDE_DIRS})
    endif(NOT TARGET ImGuizmo)
endif(ImGuizmo_FOUND)

