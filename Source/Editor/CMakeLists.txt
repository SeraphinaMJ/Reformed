cmake_minimum_required(VERSION 3.15.0)

find_package(ImGui REQUIRED)
find_package(ImGuizmo REQUIRED)
find_package(ImGuiOpenGL3GLFWGLEW REQUIRED)
find_package(RapidJSON REQUIRED)

if(UNIX)
    find_package(LinuxWinDefs REQUIRED)
endif()

add_executable(Editor WIN32
    AnimatorRenderer.hpp AnimatorRenderer.cpp
    CSVParser.hpp CSVParser.cpp
    DataSelect.hpp DataSelect.cpp
    dirent_portable.h
    EditorLogger.hpp
    EditorState.hpp
    EditorObject.hpp EditorObject.cpp
    EditorObjectManager.hpp EditorObjectManager.cpp
    ObjectSelectorManipulator.hpp
    EditorWindow.hpp EditorWindow.cpp
    ObjectSelector.hpp
    EngineController.hpp EngineController.cpp
    FileOpener.hpp FileOpener.cpp
    GraphicsWindow.hpp GraphicsWindow.cpp
    HierarchyRenderer.hpp HierarchyRenderer.cpp
    HierarchySelectionKeeper.hpp HierarchySelectionKeeper.cpp
    HierarchyWindow.hpp HierarchyWindow.cpp
    imguifilesystem.h imguifilesystem.cpp
    InspectorRenderer.hpp InspectorRenderer.cpp
    InspectorWindow.hpp InspectorWindow.cpp
    ObjectSelector.cpp
    main.cpp
    NavMeshWindow.hpp NavMeshWindow.cpp
    SceneWindow.hpp SceneWindow.cpp
    StyleKeeper.hpp StyleKeeper.cpp
    ToolbarRenderer.hpp ToolbarRenderer.cpp

    Components/AgentRenderer.hpp Components/AgentRenderer.cpp
    Components/ButtonRenderer.hpp Components/ButtonRenderer.cpp
    Components/ComponentRenderer.hpp Components/ComponentRenderer.cpp
    Components/InputRenderer.hpp Components/InputRenderer.cpp
    Components/RendererRenderer.hpp Components/RendererRenderer.cpp
    Components/RigidBodyRenderer.h Components/RigidBodyRenderer.cpp
    Components/SampleRenderer.hpp Components/SampleRenderer.cpp
    Components/AllegianceRenderer.hpp Components/AllegianceRenderer.cpp

    NodeGraph/NodeGraphWindow.hpp NodeGraph/NodeGraphWindow.cpp
    NodeGraph/NodeManager.hpp NodeGraph/NodeManager.cpp
    NodeGraph/EditorNode.hpp NodeGraph/EditorNode.cpp

    NodeGraph/GraphTypeSelector.hpp NodeGraph/GraphTypeSelector.cpp
    NodeGraph/AgentMenuSelection.hpp NodeGraph/AgentMenuSelection.cpp
    NodeGraph/NGE_Watchdog.hpp NodeGraph/NGE_Watchdog.cpp

    Renderers/GizmoRenderer.hpp Renderers/GizmoRenderer.cpp
    Renderers/ToggleButton.hpp Renderers/ToggleButton.cpp

    Scaffolding/RenderableBase.hpp Scaffolding/RenderableBase.cpp
    Scaffolding/WindowBase.hpp Scaffolding/WindowBase.cpp

    UndoRedo.hpp UndoRedo.cpp
    EditorInput.hpp EditorInput.cpp
    ObjectCommand.hpp ObjectCommand.cpp
    Command.hpp Command.cpp
    )

target_include_directories(Editor
    PRIVATE TempestEngine
    PRIVATE ImGui
    PRIVATE ${LinuxWinDefs_INCLUDES}
	PRIVATE ${RapidJSON_INCLUDE_DIRS}
)

if(UNIX)
    target_link_libraries(Editor
        PRIVATE dl
        PRIVATE pthread
        PRIVATE X11
    )
endif()

target_link_libraries(Editor
    PRIVATE Logger
    PRIVATE EngineInterfaces
    PRIVATE ComponentCore
    PRIVATE SerializationCore
    PRIVATE Math
	PRIVATE Physics.Bullet
    PRIVATE MathSerialization
    PRIVATE EngineRunner
    PRIVATE TempestEngine
    PRIVATE ComponentRepository
    PRIVATE ImGuizmo
    PRIVATE ImGui
    PRIVATE ImGui_Stdlib
    PRIVATE ImGui_OpenGL3_GLFW_GLEW
    PRIVATE BulletDynamics
    PRIVATE EventSystem
	PRIVATE EditorAdapter
)

# build up resources list
list(APPEND resources ${TempestEngine_RESOURCES})
set(Editor_RESOURCES ${resources} CACHE STRING "Editor_RESOURCES" FORCE)

# deploy resource files
foreach(resource_file ${Editor_RESOURCES})
    add_custom_command(
        TARGET Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${resource_file}
            $<TARGET_FILE_DIR:Editor>
    )
endforeach(resource_file)

set_target_properties(Editor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
