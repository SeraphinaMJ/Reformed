
if(WIN32)
    set(XInput_LIBRARIES
        XInput.lib
    )

else()
    find_package(LinuxWinDefs)

    find_path(XInput_ROOT
        NAMES XInput.c
        PATHS ${CMAKE_PREFIX_PATH}
        PATH_SUFFIXES XInput_Linux
    )

    add_library(XInput STATIC ${XInput_ROOT}/XInput.c)
    # target_link_libraries(XInput INTERFACE ${XInput_LIBRARIES})
    target_include_directories(XInput
        PUBLIC ${XInput_ROOT}
        PRIVATE ${LinuxWinDefs_INCLUDES}
    )

    set(XInput_LIBRARIES XInput)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XInput
    FOUND_VAR
        XInput_FOUND
    REQUIRED_VARS
        XInput_LIBRARIES
)

if(XInput_FOUND)
    if(NOT TARGET XInput)
        add_library(XInput INTERFACE)
        target_link_libraries(XInput INTERFACE ${XInput_LIBRARIES})
    endif(NOT TARGET XInput)
endif(XInput_FOUND)
