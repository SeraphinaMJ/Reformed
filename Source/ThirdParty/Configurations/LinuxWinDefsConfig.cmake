if(UNIX)
    find_path(LinuxWinDefs_ROOT
        NAMES windef.h
        PATHS ${CMAKE_PREFIX_PATH}
        PATH_SUFFIXES LinuxWinDefs
    )

    message("LinuxWinDefs_ROOT=${LinuxWinDefs_ROOT}")

    set(LinuxWinDefs_INCLUDES ${LinuxWinDefs_ROOT})

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(LinuxWinDefs
        FOUND_VAR
            LinuxWinDefs_FOUND
        REQUIRED_VARS
            LinuxWinDefs_INCLUDES
    )

    if(LinuxWinDefs_FOUND)
        if(NOT TARGET LinuxWinDefs)
            add_library(LinuxWinDefs STATIC
                ${LinuxWinDefs_ROOT}/synchapi.c
                ${LinuxWinDefs_ROOT}/shlobj.c
            )
            target_include_directories(LinuxWinDefs
                PUBLIC ${LinuxWinDefs_INCLUDES}
                PRIVATE ${XInput_INCLUDES}
            )

        endif(NOT TARGET LinuxWinDefs)
    endif(LinuxWinDefs_FOUND)
endif()

