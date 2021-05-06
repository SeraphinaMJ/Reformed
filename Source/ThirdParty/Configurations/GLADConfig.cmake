find_path(GLAD_ROOT 
    NAMES glad/glad.h
    PATH_SUFFIXES glad-
)


find_path(GLAD_INCLUDE_DIRS
    NAMES glad/glad.h
    PATHS ${GLAD_ROOT}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLAD
    FOUND_VAR
        GLAD_FOUND
    REQUIRED_VARS
        GLAD_INCLUDE_DIRS
)

if(GLAD_FOUND)
    if(NOT TARGET GLAD)
        add_library(GLAD INTERFACE)
        target_include_directories(GLAD INTERFACE ${GLAD_INCLUDE_DIRS})
    endif(NOT TARGET GLAD)
endif(GLAD_FOUND)

