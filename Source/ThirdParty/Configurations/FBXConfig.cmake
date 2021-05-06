find_path(FBX_ROOT
    NAMES include/fbxsdk.h
    PATH_SUFFIXES fbx-
)

find_path(FBX_INCLUDE_DIRS
    NAMES fbxsdk.h
    PATHS ${FBX_ROOT}
    PATH_SUFFIXES include
)

if(WIN32)
    find_library(FBX_LIBRARIES
        NAMES
            libfbxsdk
            libfbxsdk-mt
            libfbxsdk-md
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/vs2015/x64/release
    )

    find_file(FBX_RESOURCES
        NAMES libfbxsdk.dll
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/vs2015/x64/release
    )
else()
    find_library(FBX_LIBRARIES
        NAMES
            fbxsdk
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/gcc4/x64/release
    )

    find_file(FBX_RESOURCES
        NAMES libfbxsdk.so
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/gcc4/x64/release
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FBX
    FOUND_VAR
        FBX_FOUND
    REQUIRED_VARS
        FBX_INCLUDE_DIRS
        FBX_LIBRARIES
        FBX_RESOURCES
)

if(FBX_FOUND)
    if(NOT TARGET FBX)
        add_library(FBX INTERFACE)
        target_include_directories(FBX SYSTEM INTERFACE ${FBX_INCLUDE_DIRS})
        target_link_libraries(FBX INTERFACE ${FBX_LIBRARIES})
    endif(NOT TARGET FBX)
endif(FBX_FOUND)
