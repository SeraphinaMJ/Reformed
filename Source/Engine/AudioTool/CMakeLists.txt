cmake_minimum_required(VERSION 3.15.0)

find_package(GLAD REQUIRED)
find_package(GLFW REQUIRED)

if(WIN32)
    set(GLFW_LIBNAME GLFW)
else()
    set(GLFW_LIBNAME glfw)
endif()

add_executable(AudioTool
    AudioMetrics.h AudioMetrics.cpp
    AudioPlayer.h AudioPlayer.cpp
    AudioSystem.h AudioSystem.cpp
    DSPManipulator.h DSPManipulator.cpp
    FilterManipulator.h FilterManipulator.cpp
    main.cpp
    ObjectEditor.h ObjectEditor.cpp
    ObjectHierarchy.h ObjectHierarchy.cpp
    ProcessingDisplay.h
    SystemBase.h SystemBase.cpp
    SystemManager.h SystemManager.cpp
    UI_System.h UI_System.cpp
    UIEventTypes.h

    glad.c

    ImGui/imconfig.h
    ImGui/imgui_demo.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_impl_glfw.h ImGui/imgui_impl_glfw.cpp
    ImGui/imgui_impl_opengl3.h ImGui/imgui_impl_opengl3.cpp
    ImGui/imgui_internal.h
    ImGui/imgui_widgets.cpp
    ImGui/imgui.h ImGui/imgui.cpp
    ImGui/imstb_rectpack.h
    ImGui/imstb_textedit.h
    ImGui/imstb_truetype.h
    )

target_include_directories(AudioTool
    PRIVATE ${GLAD_INCLUDE_DIRS}
    PRIVATE GLFW
)

target_link_libraries(AudioTool
    PRIVATE Configuration
    PRIVATE AudioEngine
    PRIVATE ${GLFW_LIBNAME}
)

if(UNIX)
    target_link_libraries(AudioTool
        PRIVATE dl
        PRIVATE pthread
        PRIVATE stdc++fs
    )
endif()

# build up resources list
list(APPEND resources ${AudioEngine_RESOURCES})
list(APPEND resources ${GLFW_RESOURCES})
set(AudioTool_RESOURCES ${resources} CACHE STRING "AudioTool_RESOURCES" FORCE)

# deploy resource files
foreach(resource_file ${AudioTool_RESOURCES})
    add_custom_command(
        TARGET AudioTool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${resource_file}
            $<TARGET_FILE_DIR:AudioTool>
    )
endforeach(resource_file)
