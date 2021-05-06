find_path(STB_ROOT
    NAMES stb_image.h
    PATH_SUFFIXES stb-1.41
)

find_path(STB_INCLUDE_DIRS
    NAMES stb_image.h
    PATHS ${STB_ROOT}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(STB
    FOUND_VAR
        STB_FOUND
    REQUIRED_VARS
        STB_INCLUDE_DIRS
)

if(STB_FOUND)
    if(NOT TARGET STB)
        add_library(STB INTERFACE)
        target_include_directories(STB INTERFACE ${STB_INCLUDE_DIRS})
    endif(NOT TARGET STB)
endif(STB_FOUND)