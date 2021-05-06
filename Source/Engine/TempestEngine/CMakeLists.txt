cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../Programs/cmake-precompiled-header")
include(PrecompiledHeader)

# dependencies

find_package(GLEWInternal REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
#find_package(BulletInternal REQUIRED)
find_package(FBX REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(FreetypeInternal REQUIRED)
find_package(STB REQUIRED)

if(WIN32)
    find_package(XINPUT REQUIRED)
else()
    find_package(XInput REQUIRED)
    find_package(LinuxWinDefs REQUIRED)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -fdeclspec")
endif()

add_library(TempestEngine STATIC
    Precompiled.cpp # this MUST be the first entry in the files list (for cotire)
    Precompiled.h
    Application.hpp Application.cpp
    DLLControl.hpp
    Engine.hpp Engine.cpp
    FilterBase.h
    Factions.hpp Factions.cpp
    GameObjectCollector.hpp
    GameObjectGatherer.hpp
#    GamepadControls.hpp GamepadControls.cpp # TODO: Fix this!
    GamePadSystem.hpp GamePadSystem.cpp
    LeakDetection.hpp
    MeshDebug.hpp MeshDebug.cpp
    PersistenceManipulator.hpp PersistenceManipulator.cpp
    PersistenceManipulatorInterface.hpp
    PersistenceRequest.hpp PersistenceRequest.cpp
    PersistenceSystem.hpp PersistenceSystem.cpp
    ScriptMaps.hpp ScriptMaps.cpp
    Serialization.h Serialization.cpp
    SystemBase.hpp SystemBase.cpp
    SystemBundle.hpp SystemBundle.cpp
    SystemManager.hpp SystemManager.cpp
    SystemTask.hpp SystemTask.cpp
    TransformTestScript.hpp TransformTestScript.cpp

    Actions/Action.hpp Actions/Action.cpp
    Actions/ActionSystem.hpp Actions/ActionSystem.cpp
    Actions/DelayAction.hpp Actions/DelayAction.cpp
    Actions/LerpAction.hpp Actions/LerpAction.cpp

    Audio/AudioComponent.hpp Audio/AudioComponent.cpp
    Audio/AudioSystem.h Audio/AudioSystem.cpp

    Components/AnimatorComponent.hpp Components/AnimatorComponent.cpp
    Components/Agent.hpp Components/Agent.cpp
    Components/Button.hpp Components/Button.cpp
    Components/Camera.hpp Components/Camera.cpp
    Components/FBXFileComponent.h Components/FBXFileComponent.cpp
    Components/Health.hpp Components/Health.cpp
    Components/Allegiance.hpp Components/Allegiance.cpp
    Components/InputComponent.hpp Components/InputComponent.cpp
    Components/Light.hpp Components/Light.cpp
    Components/ParticleEmitter.hpp Components/ParticleEmitter.cpp
    Components/Renderer.hpp Components/Renderer.cpp
    Components/RenderTarget.hpp Components/RenderTarget.cpp
    Components/RigidBody.hpp Components/RigidBody.cpp
    Components/ScriptCPP.hpp Components/ScriptCPP.cpp
    Components/Skybox.hpp Components/Skybox.cpp
	  Components/UISprite.hpp Components/UISprite.cpp
	Components/UISpriteArray.hpp Components/UISpriteArray.cpp
    Components/WaypointNode.hpp Components/WaypointNode.cpp
    Components/WaypointNodeSet.hpp Components/WaypointNodeSet.cpp
    Components/Enemy.hpp Components/Enemy.cpp
    Components/Player.hpp Components/Player.cpp
    Components/MinionAll.hpp Components/MinionAll.cpp
    Components/MinionAllExceptType_1.hpp Components/MinionAllExceptType_1.cpp
    Components/MinionGroup_1.hpp Components/MinionGroup_1.cpp
    Components/MinionGroup_2.hpp Components/MinionGroup_2.cpp
    Components/MinionGroup_3.hpp Components/MinionGroup_3.cpp
    Components/MinionGroup_4.hpp Components/MinionGroup_4.cpp
    Components/MinionGroup_5.hpp Components/MinionGroup_5.cpp
    Components/MinionGroup_6.hpp Components/MinionGroup_6.cpp
    Components/MinionGroup_7.hpp Components/MinionGroup_7.cpp
    Components/MinionGroup_8.hpp Components/MinionGroup_8.cpp
    Components/MinionGroup_9.hpp Components/MinionGroup_9.cpp
    Components/MinionGroup_10.hpp Components/MinionGroup_10.cpp
    Components/MinionType_1.hpp Components/MinionType_1.cpp
    Components/MinionType_2.hpp Components/MinionType_2.cpp
    Components/MinionType_3.hpp Components/MinionType_3.cpp
    Components/MinionLeader.hpp Components/MinionLeader.cpp
    Components/EnemyLeader.hpp Components/EnemyLeader.cpp
    Components/Selected.hpp Components/Selected.cpp

    DynamicWaypoints/WaypointGraphPathfinder.hpp DynamicWaypoints/WaypointGraphPathfinder.cpp
    DynamicWaypoints/WaypointGraphPathfinder.hpp DynamicWaypoints/WaypointGraphPathfinder.cpp
    DynamicWaypoints/DynamicWaypointGraph.hpp DynamicWaypoints/DynamicWaypointGraph.cpp

    Events/CollisionEvent.hpp
    Events/TestEvent.hpp
    Events/ButtonPressedEvent.hpp

    External/AssetManipulator.hpp External/AssetManipulator.cpp
    External/AssetManipulatorInterface.hpp

    External/BehaviorTreeManipulator.hpp External/BehaviorTreeManipulator.cpp

    External/bundleManipulator.hpp External/bundleManipulator.cpp
    External/bundleManipulatorInterface.hpp
    External/CameraManipulator.hpp External/CameraManipulator.cpp
    External/CameraManipulatorInterface.hpp
    External/EngineMetadataManipulator.hpp External/EngineMetadataManipulator.cpp
    External/EngineMetadataManipulatorInterface.hpp
	External/GraphicsManipulatorInterface.hpp
	External/GraphicsManipulator.hpp External/GraphicsManipulator.cpp
    External/NodeManipulator.hpp External/NodeManipulator.cpp
    External/NodeManipulatorInterface.hpp
    External/SceneEntityReference.hpp External/SceneEntityReference.cpp
    External/SceneManipulator.hpp External/SceneManipulator.cpp
    External/SceneManipulatorInterface.hpp


    # Beginning of formations Section
    Formations/FormationsManager.hpp Formations/FormationsManager.cpp
    Formations/Squad.hpp Formations/Squad.cpp
    Formations/PlayerSquad.hpp Formations/PlayerSquad.cpp
    Formations/EnemySquad.hpp Formations/EnemySquad.cpp
    Formations/SquadRing.hpp Formations/SquadRing.cpp


    # End of formations Section

    GameObjectFiltering/ComponentTypeSet.hpp GameObjectFiltering/ComponentTypeSet.cpp
    GameObjectFiltering/GameObjectFilter.hpp GameObjectFiltering/GameObjectFilter.cpp
    GameObjectFiltering/ListBuilder.hpp GameObjectFiltering/ListBuilder.cpp

    GameScripts/AudioScript.h GameScripts/AudioScript.cpp
    GameScripts/BombScript.h GameScripts/BombScript.cpp
    GameScripts/CameraScript.hpp GameScripts/CameraScript.cpp
    GameScripts/ChangeSceneAfterDelayScript.hpp GameScripts/ChangeSceneAfterDelayScript.cpp
    GameScripts/CharacterStateScript.h GameScripts/CharacterStateScript.cpp
    GameScripts/DamagePrismScript.hpp GameScripts/DamagePrismScript.cpp
    GameScripts/DynamiteScript.h GameScripts/DynamiteScript.cpp
    GameScripts/EnemyAIScript.h GameScripts/EnemyAIScript.cpp
    GameScripts/GameManagerScript.h GameScripts/GameManagerScript.cpp
    GameScripts/GrabbableScript.hpp GameScripts/GrabbableScript.cpp
    GameScripts/GroundPoundScript.h GameScripts/GroundPoundScript.cpp
    GameScripts/InputTestScript.hpp GameScripts/InputTestScript.cpp
    GameScripts/JumpPadScript.h GameScripts/JumpPadScript.cpp
    GameScripts/LariatScript.h GameScripts/LariatScript.cpp
    GameScripts/LifetimeScript.hpp GameScripts/LifetimeScript.cpp
    GameScripts/LightTestScript.hpp GameScripts/LightTestScript.cpp
    GameScripts/menuControllerScript.hpp GameScripts/menuControllerScript.cpp
	GameScripts/MouseLookScript.hpp GameScripts/MouseLookScript.cpp
	GameScripts/PikminPlayerControllerScript.hpp GameScripts/PikminPlayerControllerScript.cpp
    GameScripts/PlayButtonScript.hpp GameScripts/PlayButtonScript.cpp
    GameScripts/PlayerControllerScript.hpp GameScripts/PlayerControllerScript.cpp
    GameScripts/PrebuildTestScript.h GameScripts/PrebuildTestScript.cpp
    GameScripts/RenderTargetScript.hpp GameScripts/RenderTargetScript.cpp
    GameScripts/ShooterScript.h GameScripts/ShooterScript.cpp
    GameScripts/SpawnerScript.h GameScripts/SpawnerScript.cpp
    GameScripts/StressTestScript.hpp GameScripts/StressTestScript.cpp
    GameScripts/TestingScript.hpp GameScripts/TestingScript.cpp
    GameScripts/TextSampleScript.hpp GameScripts/TextSampleScript.cpp
	GameScripts/ThirdPersonCameraController.hpp GameScripts/ThirdPersonCameraController.cpp
    GameScripts/PathFollowScript.hpp GameScripts/PathFollowScript.cpp
    GameScripts/TargetingCircleScript.hpp GameScripts/TargetingCircleScript.cpp
    GameScripts/PathingStateScript.hpp GameScripts/PathingStateScript.cpp
    GameScripts/ToggleDisplayOnButtonPress.hpp GameScripts/ToggleDisplayOnButtonPress.cpp
    GameScripts/ScoreTracker.hpp GameScripts/ScoreTracker.cpp
    GameScripts/ExitGameButton.hpp GameScripts/ExitGameButton.cpp
    GameScripts/CheatCodeScript.hpp GameScripts/CheatCodeScript.cpp
    GameScripts/RequestConfirmation.hpp GameScripts/RequestConfirmation.cpp
    GameScripts/KillFieldScript.hpp GameScripts/KillFieldScript.cpp

    Input/GamepadInputAdapter.hpp Input/GamepadInputAdapter.cpp

    HealthSystem/HealthManager.hpp HealthSystem/HealthManager.cpp


    Input/InputSystem.hpp Input/InputSystem.cpp

    Messaging/Message.hpp

    Nodes/Node.hpp Nodes/Node.cpp
    Nodes/NodeFactory.hpp Nodes/NodeFactory.cpp

    # Behavior Tree Section
    BehaviorSystem/Blackboard.hpp BehaviorSystem/Blackboard.cpp
    BehaviorSystem/BehaviorEnums.hpp
    BehaviorSystem/BehaviorTree.hpp BehaviorSystem/BehaviorTree.cpp
    BehaviorSystem/BehaviorTreeManager.hpp BehaviorSystem/BehaviorTreeManager.cpp
    BehaviorSystem/BehaviorTreeBuilder.hpp BehaviorSystem/BehaviorTreeBuilder.cpp
    BehaviorSystem/BehaviorTreeDataBase.hpp BehaviorSystem/BehaviorTreeDataBase.cpp
    BehaviorSystem/BehaviorTask.hpp BehaviorSystem/BehaviorTask.cpp
    BehaviorSystem/BehaviorInterpreter.hpp BehaviorSystem/BehaviorInterpreter.cpp

    BehaviorSystem/BehaviorSpecialData.hpp
    BehaviorSystem/BehaviorHelpers.hpp BehaviorSystem/BehaviorHelpers.cpp

    Nodes/Behaviors/AllBehaviorsInclude.hpp

    # base nodes
    Nodes/Behaviors/Behavior.hpp Nodes/Behaviors/Behavior.cpp
    Nodes/Node.hpp Nodes/Node.cpp
    # leaves
    Nodes/Behaviors/Leaf.hpp Nodes/Behaviors/Leaf.cpp 
    Nodes/Behaviors/AttackClosestEnemy.hpp Nodes/Behaviors/AttackClosestEnemy.cpp
    Nodes/Behaviors/BreakFormation.hpp Nodes/Behaviors/BreakFormation.cpp
    Nodes/Behaviors/DefaultLeaf.hpp Nodes/Behaviors/DefaultLeaf.cpp
    Nodes/Behaviors/GoToEnemy.hpp Nodes/Behaviors/GoToEnemy.cpp
    Nodes/Behaviors/JoinSquad.hpp Nodes/Behaviors/JoinSquad.cpp
    Nodes/Behaviors/JumpUp.hpp Nodes/Behaviors/JumpUp.cpp
    Nodes/Behaviors/MoveRandomly.hpp Nodes/Behaviors/MoveRandomly.cpp
    Nodes/Behaviors/PickEnemyToFight.hpp Nodes/Behaviors/PickEnemyToFight.cpp

    Nodes/Behaviors/ReturnToLeader.hpp Nodes/Behaviors/ReturnToLeader.cpp
    Nodes/Behaviors/Regroup.hpp Nodes/Behaviors/Regroup.cpp


    #Decorators
    Nodes/Behaviors/Decorator.hpp Nodes/Behaviors/Decorator.cpp
    Nodes/Behaviors/EnemyWithinRange.hpp Nodes/Behaviors/EnemyWithinRange.cpp
    Nodes/Behaviors/EnemySpotted.hpp Nodes/Behaviors/EnemySpotted.cpp
    Nodes/Behaviors/HasSquad.hpp Nodes/Behaviors/HasSquad.cpp
    Nodes/Behaviors/Inverter.hpp Nodes/Behaviors/Inverter.cpp
    Nodes/Behaviors/IsDead.hpp Nodes/Behaviors/IsDead.cpp
    Nodes/Behaviors/IsLowHealth.hpp Nodes/Behaviors/IsLowHealth.cpp
    Nodes/Behaviors/IsSquadEngaged.hpp Nodes/Behaviors/IsSquadEngaged.cpp
    Nodes/Behaviors/Timer.hpp Nodes/Behaviors/Timer.cpp
    Nodes/Behaviors/ReturnTrue.hpp Nodes/Behaviors/ReturnTrue.cpp
    Nodes/Behaviors/Repeater.hpp Nodes/Behaviors/Repeater.cpp
    Nodes/Behaviors/RepeatUntilN.hpp Nodes/Behaviors/RepeatUntilN.cpp
    Nodes/Behaviors/RepeatUntilFailure.hpp Nodes/Behaviors/RepeatUntilFailure.cpp
    Nodes/Behaviors/RepeatUntilSuccess.hpp Nodes/Behaviors/RepeatUntilSuccess.cpp

    #Composites
    Nodes/Behaviors/Composite.hpp Nodes/Behaviors/Composite.cpp
    Nodes/Behaviors/Selector.hpp Nodes/Behaviors/Selector.cpp
    Nodes/Behaviors/Sequencer.hpp Nodes/Behaviors/Sequencer.cpp
    # end of Behavior Tree Section



    Physics/PhysicsSystem.hpp Physics/PhysicsSystem.cpp

    Platform/PlatformWindowing.hpp Platform/PlatformWindowing.cpp

    Rendering/Buffer.hpp
    Rendering/CameraBase.hpp Rendering/CameraBase.cpp
    Rendering/CameraSystem.hpp Rendering/CameraSystem.cpp
    Rendering/CubeTexture.hpp Rendering/CubeTexture.cpp
    Rendering/FrameBuffer.hpp Rendering/FrameBuffer.cpp
    Rendering/FrameBufferManager.hpp Rendering/FrameBufferManager.cpp
    Rendering/GLDebug.hpp Rendering/GLDebug.cpp
#    Rendering/GLoader.hpp Rendering/GLoader.cpp
    Rendering/GraphicsSystem.hpp Rendering/GraphicsSystem.cpp
    Rendering/IndexBufferObject.hpp Rendering/IndexBufferObject.cpp
    Rendering/LineMesh.cpp
    Rendering/DepthTexture.hpp Rendering/DepthTexture.cpp
    Rendering/DebugLines.hpp Rendering/DebugLines.cpp
    Rendering/LineBuffer.hpp Rendering/LineBuffer.cpp
    Rendering/LineSegment.hpp Rendering/LineSegment.cpp
    Rendering/Material.hpp Rendering/Material.cpp
#    Rendering/MaterialManager.hpp Rendering/MaterialManager.cpp
    Rendering/MaterialLoader.hpp Rendering/MaterialLoader.cpp
    Rendering/Mesh.hpp Rendering/Mesh.cpp
    Rendering/MeshBuilder.hpp Rendering/MeshBuilder.cpp
    Rendering/ModularFrameBuffer.hpp Rendering/ModularFrameBuffer.cpp
    Rendering/ParticleBuffer.hpp Rendering/ParticleBuffer.cpp
    Rendering/ParticleMesh.hpp Rendering/ParticleMesh.cpp
    Rendering/ParticleSystem.hpp Rendering/ParticleSystem.cpp
    Rendering/ProgramPipeline.hpp Rendering/ProgramPipeline.cpp
    Rendering/Shader.hpp Rendering/Shader.cpp
    Rendering/ShaderManager.hpp Rendering/ShaderManager.cpp
    Rendering/ShaderProgram.hpp Rendering/ShaderProgram.cpp
    Rendering/SimpleTexture.hpp Rendering/SimpleTexture.cpp
    Rendering/TextRenderer.hpp Rendering/TextRenderer.cpp
    Rendering/Texture.hpp Rendering/Texture.cpp
    Rendering/TextureManager.hpp Rendering/TextureManager.cpp
    Rendering/TriangleMesh.hpp Rendering/TriangleMesh.cpp
    Rendering/TypeData.hpp
    Rendering/VertexArrayManager.hpp Rendering/VertexArrayManager.cpp
    Rendering/VertexArrayObject.hpp Rendering/VertexArrayObject.cpp
    Rendering/VertexBufferObject.hpp Rendering/VertexBufferObject.cpp
    Rendering/ShadowAtlas.hpp Rendering/ShadowAtlas.cpp
    Rendering/ShaderLoader.hpp Rendering/ShaderLoader.cpp
    Rendering/GLTypes.hpp
    #Rendering/Windowing.hpp Rendering/Windowing.cpp # TODO: Fix this!
    Rendering/ParticleRenderer.hpp Rendering/ParticleRenderer.cpp

    Replay/InstantReplaySystem.hpp Replay/InstantReplaySystem.cpp
    Replay/JumbotronComponent.hpp Replay/JumbotronComponent.cpp

    SceneManagement/ComponentList.hpp SceneManagement/ComponentList.cpp
    SceneManagement/GameObject.hpp SceneManagement/GameObject.cpp
    SceneManagement/GameObjectSerialization.hpp SceneManagement/GameObjectSerialization.cpp
    SceneManagement/Scene.hpp SceneManagement/Scene.cpp
    SceneManagement/SceneSystem.hpp SceneManagement/SceneSystem.cpp
    SceneManagement/Space.hpp SceneManagement/Space.cpp
    SceneManagement/Transform.hpp SceneManagement/Transform.cpp
    SceneManagement/UISystem.hpp SceneManagement/UISystem.cpp

    Scripting/ScriptingSystem.hpp Scripting/ScriptingSystem.cpp

    SystemManagement/SystemManagerInterface.hpp
    SystemManagement/SystemManagerSTA.hpp SystemManagement/SystemManagerSTA.cpp
    SystemManagement/SystemMonitor.hpp SystemManagement/SystemMonitor.cpp

    Systems/AnimationSystem.hpp Systems/AnimationSystem.cpp
    Systems/FBXLoader.hpp Systems/FBXLoader.cpp

    Telemetry/InjectableInterface.hpp Telemetry/InjectableInterface.cpp
    Telemetry/QueryableInterface.hpp
    Telemetry/SQLConnection.hpp Telemetry/SQLConnection.cpp
    Telemetry/TelemetryComponent.hpp Telemetry/TelemetryComponent.cpp
    Telemetry/TelemetrySystem.hpp Telemetry/TelemetrySystem.cpp

    Threading/JobManager.hpp Threading/JobManager.cpp
    Threading/Task.hpp Threading/Task.cpp

    Utility/AssetManager.hpp Utility/AssetManager.cpp
    Utility/Debug.hpp Utility/Debug.cpp
    Utility/GeneralizedFileLoader.hpp Utility/GeneralizedFileLoader.cpp
    Utility/NavMeshGenerator.hpp Utility/NavMeshGenerator.cpp
    Utility/TextureLoader.hpp Utility/TextureLoader.cpp
    ComponentFactory.cpp
    ComponentFactory.hpp
    ComponentLists.h
    CollisionLayer.hpp
    )

target_include_directories(TempestEngine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC Actions
    PUBLIC Audio
    PUBLIC Components
    PUBLIC Events
    PUBLIC External
    PUBLIC GameObjectFiltering
    PUBLIC GameScripts
    PUBLIC Input
    PUBLIC Logging
    PUBLIC Math
    PUBLIC Messaging
    PUBLIC Metadata
    PUBLIC Physics
    PUBLIC Platform
    PUBLIC Prebuild
    PUBLIC Rendering
    PUBLIC Replay
    PUBLIC SceneManagement
    PUBLIC Scripting
    PUBLIC SystemManagement
    PUBLIC Systems
    PUBLIC Telemetry
    PUBLIC Threading
    PUBLIC Utility
    PRIVATE ${GLM_INCLUDE_DIRS}
    PRIVATE ${RapidJSON_INCLUDE_DIRS}
    PRIVATE ${STB_INCLUDE_DIRS}
    PRIVATE ${LinuxWinDefs_INCLUDES}
)

if(UNIX)
    target_link_libraries(TempestEngine
        PRIVATE stdc++fs
        PRIVATE LinuxWinDefs
        PRIVATE ${XInput_LIBRARIES}
    )
endif()

target_link_libraries(TempestEngine
    PRIVATE Logger
    PRIVATE Math
    PRIVATE MathSerialization
    PUBLIC Configuration
    PUBLIC EngineInterfaces
    PUBLIC SerializationCore
    PUBLIC ComponentCore
    PRIVATE SystemCore
    PRIVATE Graphics.OpenGL4
    PRIVATE Physics.Bullet
    PRIVATE AudioEngine
    PRIVATE GLEWInternal
    PRIVATE FBX
    PRIVATE FreetypeInternal
    #PRIVATE BulletInternal
    PRIVATE BulletCollision
    PRIVATE BulletDynamics
    PRIVATE LinearMath
    PRIVATE GLFW
    PRIVATE XInput
    PRIVATE ComponentRepository
    PRIVATE EventSystem
	PRIVATE InputFramework
)


# build up resources list
list(APPEND resources ${Graphics.OpenGL4_RESOURCES})
list(APPEND resources ${AudioEngine_RESOURCES})
list(APPEND resources ${GLEWInternal_RESOURCES})
list(APPEND resources ${FBX_RESOURCES})
list(APPEND resources ${FreetypeInternal_RESOURCES})
list(APPEND resources ${GLFW_RESOURCES})
set(TempestEngine_RESOURCES ${resources} CACHE STRING "TempestEngine_RESOURCES" FORCE)


set_target_properties(TempestEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

## setup the precompiled header
add_precompiled_header(TempestEngine Precompiled.h
    SOURCE_CXX Precompiled.cpp FORCEINCLUDE)
