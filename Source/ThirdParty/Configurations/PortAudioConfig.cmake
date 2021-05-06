
#set(PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

#set(PortAudio_core_INCLUDE_DIRS ${PREFIX}/Include)
#set(PortAudio_core_LIBRARIES ${PREFIX}/Lib/portaudio_x64.lib)


# Adapted from: https://learning.oreilly.com/library/view/cmake-cookbook/9781788470711/df0bbf22-8b10-4e14-83f3-28ff8a6e88db.xhtml

find_path(PortAudio_ROOT 
    NAMES include/portaudio.h
    PATH_SUFFIXES portaudio-
)


find_path(PortAudio_INCLUDE_DIRS
    NAMES portaudio.h
    PATHS ${PortAudio_ROOT}
    PATH_SUFFIXES include
)

if(WIN32)
    find_library(PortAudio_LIBRARIES
        NAMES portaudio_x64
        PATHS ${PortAudio_ROOT}
        PATH_SUFFIXES Lib
    )

    find_file(PortAudio_RESOURCES
        NAMES portaudio_x64.dll
        PATHS ${PortAudio_ROOT}
    )
else()
    find_library(PortAudio_LIBRARIES
        NAMES portaudio
    )
    message("PortAudio_LIBRARIES=${PortAudio_LIBRARIES}")

    # Unlike Windows, we can just link directly against the .so, as well as
    #  distribute it
    set(PortAudio_RESOURCES "${PortAudio_LIBRARIES}")
    message("PortAudio_RESOURCES=${PortAudio_RESOURCES}")

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PortAudio
    FOUND_VAR
        PortAudio_FOUND
    REQUIRED_VARS
        PortAudio_INCLUDE_DIRS
        PortAudio_LIBRARIES
        PortAudio_RESOURCES
)

if(PortAudio_FOUND)
    if(NOT TARGET PortAudio)
        add_library(PortAudio INTERFACE)
        target_include_directories(PortAudio INTERFACE ${PortAudio_INCLUDE_DIRS})
        target_link_libraries(PortAudio INTERFACE ${PortAudio_LIBRARIES})
    endif(NOT TARGET PortAudio)
endif(PortAudio_FOUND)
