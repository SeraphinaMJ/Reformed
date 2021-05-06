
find_path(GLEWInternal_ROOT
    NAMES GL/glew.h
    PATH_SUFFIXES glew-2.1.0
)

if(WIN32)
    find_path(GLEWInternal_INCLUDE_DIRS
        NAMES GL/glew.h
        PATHS ${GLEWInternal_ROOT}
    )

    find_library(GLEWInternal_LIBRARIES
        NAMES glew32.lib
        PATHS ${GLEWInternal_ROOT}
    )

    find_file(GLEWInternal_RESOURCES
        NAMES glew32.dll
        PATHS ${GLEWInternal_ROOT}
    )
else()
    find_package(GLEW REQUIRED)

    set(GLEWInternal_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS})
    set(GLEWInternal_LIBRARIES ${GLEW_LIBRARIES})
    set(GLEWInternal_RESOURCES ${GLEWInternal_ROOT}/dummy.txt)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEWInternal
    FOUND_VAR
        GLEWInternal_FOUND
    REQUIRED_VARS
        GLEWInternal_INCLUDE_DIRS
        GLEWInternal_LIBRARIES
        GLEWInternal_RESOURCES
)

if(GLEWInternal_FOUND)
    if(NOT TARGET GLEWInternal)
        add_library(GLEWInternal INTERFACE)
        target_include_directories(GLEWInternal INTERFACE ${GLEWInternal_INCLUDE_DIRS})
        target_link_libraries(GLEWInternal INTERFACE ${GLEWInternal_LIBRARIES})
    endif(NOT TARGET GLEWInternal)
endif(GLEWInternal_FOUND)
