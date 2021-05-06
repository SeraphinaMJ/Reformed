cmake_minimum_required(VERSION 3.15.0)


find_package(GLFW REQUIRED)
if(UNIX)
    find_package(LinuxWinDefs REQUIRED)
endif()
find_package(CLI REQUIRED)

add_executable(Bootstrap WIN32
    Main.cpp
	icon.rc
)

target_include_directories(Bootstrap
    PRIVATE Graphics.OpenGL4
    PRIVATE TempestEngine
    PRIVATE GLFW
    PRIVATE ${LinuxWinDefs_INCLUDES}
)

if(UNIX)
    target_link_libraries(Bootstrap
        PRIVATE dl
        PRIVATE pthread
        PRIVATE stdc++fs
        PRIVATE X11
        PRIVATE LinuxWinDefs
    )
endif()

target_link_libraries(Bootstrap
    PRIVATE EngineInterfaces
    PRIVATE Graphics.OpenGL4
    PRIVATE EngineRunner
    PRIVATE GLFW
    PRIVATE CLI
    PRIVATE BulletDynamics
    PRIVATE ComponentRepository
    PRIVATE EventSystem
)

# build up resources list
list(APPEND resources ${Graphics.OpenGL4_RESOURCES})
list(APPEND resources ${TempestEngine_RESOURCES})
list(APPEND resources ${GLFW_RESOURCES})
set(Bootstrap_RESOURCES ${resources} CACHE STRING "Bootstrap_RESOURCES" FORCE)

# deploy resource files
foreach(resource_file ${Bootstrap_RESOURCES})
    add_custom_command(
        TARGET Bootstrap POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${resource_file}
            $<TARGET_FILE_DIR:Bootstrap>
    )
endforeach(resource_file)

set_target_properties(Bootstrap PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
    OUTPUT_NAME ${BOOTSTRAP_OUTPUT_NAME}
)

