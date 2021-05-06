find_path(FreetypeInternal_ROOT
    NAMES include/freetype/freetype.h
    PATH_SUFFIXES freetype-2.9.1
)

find_path(FreetypeInternal_INCLUDE_DIRS
    NAMES freetype/freetype.h
    PATHS ${FreetypeInternal_ROOT}
    PATH_SUFFIXES include
)

find_library(FreetypeInternal_LIBRARIES
    NAMES freetype
    PATHS ${FreetypeInternal_ROOT}
    PATH_SUFFIXES lib
)

find_file(FreetypeInternal_RESOURCES
    NAMES freetype.dll
    PATHS ${FreetypeInternal_ROOT}
    PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FreetypeInternal
    FOUND_VAR
        FreetypeInternal_FOUND
    REQUIRED_VARS
        FreetypeInternal_INCLUDE_DIRS
        FreetypeInternal_LIBRARIES
        FreetypeInternal_RESOURCES
)

if(FreetypeInternal_FOUND)
    if(NOT TARGET FreetypeInternal)
        add_library(FreetypeInternal INTERFACE)
        target_include_directories(FreetypeInternal INTERFACE ${FreetypeInternal_INCLUDE_DIRS})
        target_link_libraries(FreetypeInternal INTERFACE ${FreetypeInternal_LIBRARIES})
    endif(NOT TARGET FreetypeInternal)
endif(FreetypeInternal_FOUND)

