Design
======

System to Component Dependency Matrix (Ideal)
---------------------------------------------

==========  ========  =====  =======  ======  ======  =====  =====  =========  ===========  =======
Component   Systems
----------  ---------------------------------------------------------------------------------------
.           Graphics  Input  Gamepad  Camera  Action  Audio  Event  Messaging  Persistence  Physics
==========  ========  =====  =======  ======  ======  =====  =====  =========  ===========  =======
Renderable  Read      .      .        .
DebugDraw   Read      .      .        .     
Camera      Read      .      .        Write
InputMap    .         Write  Write    .
==========  ========  =====  =======  ======  ======  =====  =====  =========  ===========  =======


Tempest Engine Structure
------------------------

stuff

.. uml::

    @startuml
    set namespaceSeparator ::
    !include Structure/Engine/Core-ClassDiagram.iuml
    !include Structure/Engine/SystemManagement-ClassDiagram.iuml
    !include Structure/Engine/External-ClassDiagram.iuml
    !include Structure/Engine/GameObjectFiltering-ClassDiagram.iuml
    !include Structure/Engine/SceneManagement-ClassDiagram.iuml
    !include Structure/Engine/Input-ClassDiagram.iuml
    !include Structure/Engine/Serialization-ClassDiagram.iuml
    !include Structure/Engine/Animation-ClassDiagram.iuml
    !include Structure/Engine/Communications-ClassDiagram.iuml
    !include Structure/Engine/Scripting-ClassDiagram.iuml
    !include Structure/Graphics.OpenGL4/Rendering-ClassDiagram.iuml
    !include Structure/Graphics.OpenGL4/Camera-ClassDiagram.iuml
    !include Structure/Graphics.OpenGL4/UI-ClassDiagram.iuml
    !include Structure/Graphics.OpenGL4/Particles-ClassDiagram.iuml
    !include Structure/Physics.Bullet/System-ClassDiagram.iuml

    class ComponentFactory {
        {static} +GetComponentTypeDetails()
        {static} +GetScriptTypeDetails()
        {static} +SerializeComponents()
        {static} +AddComponentByType()
        {static} +RemoveComponentByType()
    }
    ComponentFactory --> SceneManagement::GameObject

    class PlatformWindowing {
        {static} +GetName()
        +Name()
        +PresentFrame()
        #OnInitialize()
        #OnStartFrame()
        #OnEndFrame()
    }
    PlatformWindowing --|> SystemManagement::SystemBase
    PlatformWindowing o-> External::ExternalWindowInterface : m_window_ptr
    PlatformWindowing o-> External::RenderContextInterface : m_render_context_ptr

    @enduml


GameObject Management Diagram
`````````````````````````````

.. uml::

    @startuml
    set namespaceSeparator ::
    !include Structure/GameObjectManagement-ClassDiagram.iuml

    @enduml

ToyBox Editor Structure
-----------------------

Editor Core
```````````

.. uml::

    @startuml
    set namespaceSeparator ::

    namespace Editor::Core {
        class Main {
            +WinMain()
        }
        class ExternalWindowInterfaceAdapter
        class RenderContextInterfaceAdapter
        class InputSourceInterfaceAdapter
        class EditorLogger {
            +Clear()
            +AddLog()
            +Draw()
        }
        class EditorWindow {
            +GetEngineController()
            +GetSceneWindow()
            +Render()
            +GetHierarchyWindow()
            +GetGizmoRenderer()
            +GetSelectionKeeper()
            +GetStyleKeeper()
            +IsRunning()
            +GetEditorState()
            +RequestShutdown()
            +GetIsProjectFolderSet()
            +SetIsProjectFolderSet()
            +GetLogger()
            +GetDarkMode()
            +SetDarkMode()
            +Shutdown()
        }
        class ToolbarRenderer {
            +Render()
        }
        class EditorState
        class HierarchySelectionKeeper {
            +GetSelectionType()
            +GetSelectionID()
            +GetSelectionName()
            +ClearSelection()
            +IsNoneSelected()
            +IsSceneSelected()
            +IsSpaceSelected()
            +IsGameObjectSelected()
            +IsSelected()
            +SetSelected()
        }
        class HierarchyRenderer {
            #onRender()
        }
        class GizmoRenderer {
            +PullData()
            +PushData()
            +RenderInWindow()
            +RenderProperties()
            +GetUsingGizmo()
            +SetDimensions()
            +SetOperation()
            +GetOperation()
            +SetMode()
            +GetMode()
            +GetOperationRef()
            +GetModeRef()
            -OnRender()
        }
        class EngineController {
            +SetEngine()
            +GetEngine()
            +ReleaseEngine()
            +ProcessEvents()
            +Start()
            +Stop()
            +IsPlaying()
        }
        class HierarchyWindow {
            #OnRender()
        }
        class SceneWindow {
            +SetImageSize()
            +SetTextureID()
            #OnRender()
        }
        class DataSelect {
            +Reset()
            +Refresh()
            #OnRender()
        }
        class InspectorWindow {
            #OnRender()
        }
        class NavMeshWindow {
            #OnRender()
        }
        class WindowBase {
        }
        class ToggleButton {
            {static} +NOOP()
            +IsEnabled()
            +Reset()
            +GetWidthRatio()
            +SetWidthRatio()
            +GetOnColor()
            +SetOnColor()
            +GetOffColor()
            +SetOffColor()
            +GetOnColorHighlight()
            +SetOnColorHighlight()
            +GetOffColorHighlight()
            +SetOffColorHighlight()
            +Render()
        }
        class StyleKeeper {
            +GetStyle()
            +GetStyles()
            +SetStyle()
            +ClearStyle()
        }
        class InspectorRenderer {
            +SetFilterOption()
            +SetRemoveHandler()
            +RenderGameObject()
        }
        class RenderableBase {
            +Render()
            +IsRunning()
            +GetLogger()
            #OnRender()
            #GetTopWindow()
            #GetSelectionKeeper()
            #GetStyleKeeper()
            #GetEditorState()
        }
        class ComponentRenderer {
            +Render()
            +OverridesMember()
            +OnRender()
        }
        class ButtonRenderer {
            #OnRender()
        }
        class RigidBodyRenderer {
            #OnRender()
        }
        class RendererRenderer {
            #OnRender()
        }
        class InputRenderer {
            #OnRender()
        }
        class SampleRenderer {
            #OnRender()
        }
        Main *-right-> ExternalWindowInterfaceAdapter
        Main *-right-> RenderContextInterfaceAdapter
        Main *-right-> InputSourceInterfaceAdapter
        Main *-down-> EditorLogger
        Main *-down-> EditorWindow
        ExternalWindowInterfaceAdapter -up-|> External::ExternalWindowInterface
        RenderContextInterfaceAdapter -up-|> External::RenderContextInterface
        InputSourceInterfaceAdapter -up-|> External::InputSourceInterface
        EditorWindow *-> ToolbarRenderer : m_toolbar_renderer
        EditorWindow *-down-> EditorState : m_current_state
        EditorWindow *-down-> HierarchySelectionKeeper : m_hierarchy_selection
        EditorWindow *-down-> GizmoRenderer : m_gizmo_renderer
        EditorWindow *-down-> EngineController : m_engine_controller
        EditorWindow *-down-> StyleKeeper : m_styles
        EditorWindow *-> HierarchyWindow : m_hierarchy_window
        EditorWindow *-> SceneWindow : m_scene_window
        EditorWindow *-> DataSelect : m_data_select
        EditorWindow *-> InspectorWindow : m_inspector_window
        EditorWindow *-> NavMeshWindow : m_nav_mesh_window
        HierarchySelectionKeeper *-> EditorLogger : m_logger
        HierarchyRenderer -up-|> RenderableBase
        GizmoRenderer -up-|> RenderableBase
        EngineController o-> Core::Engine : m_engine
        HierarchyWindow -up-|> WindowBase
        HierarchyWindow *-> HierarchyRenderer : m_renderer
        SceneWindow -up-|> WindowBase
        SceneWindow *-> ToggleButton : m_play_button
        DataSelect -up-|> WindowBase
        InspectorWindow -up-|> WindowBase
        InspectorWindow *-> InspectorRenderer : m_renderer
        NavMeshWindow -up-|> WindowBase
        WindowBase -up-|> RenderableBase
        InspectorRenderer -up-|> RenderableBase
        InspectorRenderer *-down-> "*" ComponentRenderer : m_component_renderers
        RenderableBase o-up-> EditorWindow : m_top_window
        ComponentRenderer -up-|> RenderableBase
        ButtonRenderer -up-|> ComponentRenderer
        RigidBodyRenderer -up-|> ComponentRenderer
        RendererRenderer -up-|> ComponentRenderer
        InputRenderer -up-|> ComponentRenderer
        SampleRenderer -up-|> ComponentRenderer
        AnimatorRenderer -up-|> ComponentRenderer
    }
    @enduml

Editor Manipulators
```````````````````    

.. uml::

    @startuml
    set namespaceSeparator ::
    namespace External::Manipulators {
        interface AssetManipulatorInterface {
            +AssetList()
            +StringFromWide()
        }
        interface BundleManipulatorInterface {
            +EnableBundle()
            +SuspendBundle()
            +DisableBundle()
            +GetAllBundles()
            +GetActiveBundles()
        }
        interface CameraManipulatorInterface {
            +ReadCameraMatrices()
            +EnableEditorCamera()
            +DisableEditorCamera()
            +PanCameraPosition()
            +RotateCmaera2DCoords()
            +TransCameraXZCoords()
        }
        interface EngineMetadataManipulatorInterface {
            +GetComponentTypes()
            +GetComponentType()
            +HasComponentType()
        }
        interface SceneManipulatorInterface {
            +GetSceneIDs()
            +GetSceneName()
            +GetSpaceIDsForScene()
            +GetSpaceName()
            +GetObjectIDsForSpace()
            +GetTopObjectIDsForSpace()
            +GetChildObjectIDsForObject()
            +GetObjectName()
            +GetTypeRT()
            +ApplyTypeRT()
            +AddEmptyScene()
            +AddEmptySpace()
            +AddEmptyGameObject()
            +AddRenderGameObject()
            +AddGameObjectComponent()
            +RemovegameObjectComponent()
            +ReadObjectTransform()
            +WriteObjectTransform()
            +CreateEditorCamera()
            +RemoveEditorCamera()
            +SetButtonNeighbor()
            +RemoveButtonNeighbor()
            +GetObjectID()
            +SetCollisionLayer()
            +RemoveGameObject()
            +AddParentToGameObject()
            +RemoveParent()
            +SetGameObjectName()
            +SetSceneName()
            +SetSpaceName()
            +DeleteSpace()
            +SetButtonMaterial()
            +MoveObjectToSpace()
        }
        interface PersistenceManipulatorInterface {
            +SaveCurrentScene()
            +SaveCurrentSceneAs()
            +Load()
            +New_Scene()
            +QuickStoreTo()
            +QuickRestoreFrom()
        }
    }
    namespace Core {
        class Engine {
            +GetSceneManipulator()
            +GetBundleManipulator()
            +GetPersistenceManipulator()
            +GetEngineMetadataManipulator()
            +GetCameraManipulator()
            +GetAssetManipulator()
        }
        Engine --> External::Manipulators::SceneManipulatorInterface
        Engine --> External::Manipulators::BundleManipulatorInterface
        Engine --> External::Manipulators::PersistenceManipulatorInterface
        Engine --> External::Manipulators::EngineMetadataManipulatorInterface
        Engine --> External::Manipulators::CameraManipulatorInterface
        Engine --> External::Manipulators::AssetManipulatorInterface
    }
    @enduml

Module Structure
----------------

.. uml::

    @startuml
    package "Deliverables" {
        [AudioTool]
        [Bootstrap]
        [Editor]
        [UnitTests]
    }
    [AudioEngine]
    [ComponentCore]
    [EngineInterfaces]
    [Graphics.OpenGL4]
    [Logger]
    [Physics.Bullet]
    [SerializationCore]
    [TempestEngine]
    [Utility]
    package "Third Party" {
        [BulletInternal]
        [CLI]
        [FBX]
        [FreetypeInternal]
        [GLAD]
        [GLEWInternal]
        [GLFW]
        [GLM]
        [gtest]
        [gtest_main]
        [ImGui]
        [ImGui_OpenGL3_GLFW_GLEW]
        [ImGui_Stdlib]
        [ImGuizmo]
        [PortAudio]
        [RapidJSON]
        [STB]
        [XInput]
    }
    [AudioTool] -down-> [GLAD]
    [AudioTool] -down-> [GLFW]
    [AudioTool] -down-> [AudioEngine]
    [Bootstrap] -down-> [Graphics.OpenGL4]
    [Bootstrap] -down-> [TempestEngine]
    [Bootstrap] -down-> [GLFW]
    [Bootstrap] -down-> [EngineInterfaces]
    [Bootstrap] -down-> [CLI]
    [Editor] -down-> [TempestEngine]
    [Editor] -down-> [ImGui]
    [Editor] -down-> [Logger]
    [Editor] -down-> [EngineInterfaces]
    [Editor] -down-> [ComponentCore]
    [Editor] -down-> [SerializationCore]
    [Editor] -down-> [ImGuizmo]
    [Editor] -down-> [ImGui_Stdlib]
    [Editor] -down-> [ImGui_OpenGL3_GLFW_GLEW]
    [UnitTests] -down-> [gtest]
    [UnitTests] -down-> [gtest_main]
    [UnitTests] -down-> [GLM]
    [UnitTests] -down-> [TempestEngine]

    [AudioEngine] -down-> [PortAudio]
    [ComponentCore] -down-> [SerializationCore]
    [Graphics.OpenGL4] -down-> [GLEWInternal]
    [Graphics.OpenGL4] -down-> [GLFW]
    [Graphics.OpenGL4] -down-> [TempestEngine]
    [Graphics.OpenGL4] -down-> [EngineInterfaces]
    [Physics.Bullet] -down-> [BulletInternal]
    [Physics.Bullet] -down-> [TempestEngine]
    [Physics.Bullet] -down-> [Logger]
    [Physics.Bullet] -down-> [ComponentCore]
    [Physics.Bullet] -down-> [SerializationCore]
    [TempestEngine] -down-> [GLM]
    [TempestEngine] -down-> [RapidJSON]
    [TempestEngine] -down-> [STB]
    [TempestEngine] -down-> [Graphics.OpenGL4]
    [TempestEngine] -down-> [Physics.Bullet]
    [TempestEngine] -down-> [AudioEngine]
    [TempestEngine] -down-> [GLEWInternal]
    [TempestEngine] -down-> [BulletInternal]
    [TempestEngine] -down-> [FBX]
    [TempestEngine] -down-> [FreetypeInternal]
    [TempestEngine] -down-> [GLFW]
    [TempestEngine] -down-> [Logger]
    [TempestEngine] -down-> [EngineInterfaces]
    [TempestEngine] -down-> [SerializationCore]
    [TempestEngine] -down-> [ComponentCore]
    [SerializationCore] -down-> [Logger]
    [SerializationCore] -down-> [Utility]
    @enduml

By System Structure
-------------------

Engine Core
```````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/Core-ClassDiagram.iuml
    @enduml


Engine External
```````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/External-ClassDiagram.iuml
    @enduml


Engine SystemManagement
```````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/SystemManagement-ClassDiagram.iuml
    @enduml


Engine SceneManagement
``````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/SceneManagement-ClassDiagram.iuml
    @enduml


Engine GameObjectFiltering
``````````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/GameObjectFiltering-ClassDiagram.iuml
    @enduml


Engine Serialization
````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/Serialization-ClassDiagram.iuml
    @enduml


Graphics.OpenGL4 Rendering
``````````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Graphics.OpenGL4/Rendering-ClassDiagram.iuml
    @enduml


Graphics.OpenGL4 Camera
```````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Graphics.OpenGL4/Camera-ClassDiagram.iuml
    @enduml


Graphics.OpenGL4 UI
```````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Graphics.OpenGL4/UI-ClassDiagram.iuml
    @enduml


Graphics.OpenGL4 Particles
``````````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Graphics.OpenGL4/Particles-ClassDiagram.iuml
    @enduml


Engine Input
````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/Input-ClassDiagram.iuml
    @enduml


Engine Communications
`````````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/Communications-ClassDiagram.iuml
    @enduml


Engine Animation
````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/Animation-ClassDiagram.iuml
    @enduml


Engine Scripting
````````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Engine/Scripting-ClassDiagram.iuml
    @enduml


Physics Bullet
``````````````

.. uml::

    @startuml
    skinparam linetype ortho
    set namespaceSeparator ::
    !include Structure/Physics.Bullet/System-ClassDiagram.iuml
    @enduml

