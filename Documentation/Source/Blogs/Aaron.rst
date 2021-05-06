Aaron's Blog
------------

The Project Break-down Rabbit Hole
``````````````````````````````````

In the beginning, the Tempest Engine was designed to a be an Entity-Component-System (ECS) system.  This original goal governed how systems were being developed and utilized.  It was intended that systems could be built entirely outside of the engine and introduced to the engine at runtime, leaving the engine to be purely a system pump.

Building a system outside of the engine allows the developer to build simplified unit tests and exercise their system without the need for an engine.  Their system could be shared between projects or extended to create hybrids.  The developer would also be required to construct and maintain a very clean interface to their system with components being the method of communication.

In the ECS system, components do not have behavior.  Thus, they are easily defined in portable header files and can be utilized almost universally.  The behavior we need is located inside systems, (the S in ECS).  These systems do not have any data, but only behavior, relying on the components to hold the needed information.

However, due to changes in personnel and limited developer knowledge, the original ECS focus was lost shortly after project start.  The pressures of the immature game development process quickly pushed aside adherence to guiding principles in favor of feature release.  Thus, systems quickly became intertwined with each other and basic structure code.  Additionally, behavior and dependencies were being added to the components, further spreading coupling issues.

An effort has been rekindled to form the engine into the ECS pattern again.  The idea being that a systematic massaging of the systems could allow most systems to be extracted from the basic engine.  In particular, circular dependencies would need to be severed and a solid dependency tree established.

.. Show Dependency graph from end of last year

As is evidenced by the dependency graph, there are some systems which managed to stay mostly separated from the engine codebase.  These systems were introduced very early in the project and were maintained by a single developer.

New systems, however, were added directly into the engine project.  It became too easy to just include something you **might** need.  Some header include list grew to enormous size.  This issue was compounded by some developer tools that would automatically add header includes for types, but didn't include the correct, lowest-level header; they would just include what appeared to be the **first** header that contained the proper type.  As our headers were already enormous, the tool usually added the wrong header, further perpetuating the problem.

As many experienced C/C++ developers know, the more headers you have, the slower your compile times will be; and we learned this lesson the hard way.  Compile times of the engine project alone reached over 5 minutes.  A single change to a specialized header may trigger recompilation of over 30 compilation units.

Basic Targets
`````````````

In the effort to simplify the structure of the engine and editor, some Targets were identified based on criticality and simplicity.  Critical targets are described as large systems that may need to be replaced in the near future.  We were able to see these targets very early in the engine development and had already started to make them removable.  The graphics system, built on top of OpenGL technology, was original scheduled to be replaced with a DirectX version, as part of a migration to the XBox Creators program.  As such, the functional API to the graphic system was intentionally keep limited.  All attempts to use the graphics system were restructured to be data within the renderer component.

.. show logical structure of Graphics package at end of last semester.

Similar to the graphics system, the physics system implementation was questionable through much of the development process.  Due to this insecurity, contingency plans were made to build a custom physics engine.  As part of this plan, usage of the physics system was again isolated to a small facade.  Thusly, the physics system became self-contained and free to change internal implementation at any time.

.. show logical structure of Physics package at end of last semester.




Chronological Discovery
```````````````````````

2019.09.23
''''''''''

The breakdown of the engine continues.  Small systems/modules have been extracted from the engine, but some of the key pieces are still lodged firmly.

Math, Engine Interfaces and new systems were easily moved.  Most significant issues arose from the haphazard use of header files.  A significant portion of header file includes within extracted headers were not required.  Many type references were replaced with forward declares.  In some cases, the type of parameters and members was changed from an instance to a pointer or reference.  References and pointers can be declared using forward declares as an incomplete type is valid for the compiler to continue processing.  But any form of copying requires the entire header to be included.

Numerous times when cleaning out one included header file, the compiler would alert me to additional missing type definitions.  What was happening is that the header removed actually itself included other header files, allow the other, possibly unrelated types, to be included into the target header.  Fixing this compilation problem was as simple as adding the appropriate header include, perpetuating the need to restart my analysis now that the list of header includes has changed.

The time required to clean-up a small module isn't really significant, ranging from one hour to four hours.  Since the compiler is doing most of the work, it is simple to do this work iteratively.  I was able to notice drastically improved compile times as these iterations progressed.  And, once a module has been extracted, it will only need to be compiled whenever its limited content changes; a huge benefit to a large code base.

Core systems, like SystemBase, Events and SceneManagement cannot be easily moved.  Their implementation is heavily coupled with other systems or with the engine loop itself.  I have tried to extract the SystemBase into a module at least four separate times.  Each time, I've been blocked by the SystemBase's use of another system.  As such, this is creating a circular dependency preventing the extraction of a single system.  I have to break the circular dependency issue first before  the extraction can continue.  While it is possible I could break both systems out into a single module in order to delay their final separation, this would create a lot of confusion about which module a system may be found in.  Introducing such confusion is not worth the temporary advantage.

.. Show diagram of SystemBase dependency with EventSystem. (highlight circular dependency)

At the end of the day, the though begins to creep into my mind is "Is this even worth the time and effort?"  The software engineer in me screams "Hell YES!" but the game developer side says "No, you're delaying shipping."  This struggle is a growing pain, frustration experienced by not knowing a valid path to satisfy both constraints.  Surely someone has run into this issue before and has found solutions.  And, smiling, I do know of some.

The refactoring process is needed on large code bases, whether they be games, real-time systems or financial applications.  On my desk sits Martin Fowler's *Refactoring* book which describes exactly how to deal with systematic restructuring of existing code.  Existing code is the majority of code that I will encounter in my profession, so it is worthwhile to learn how to deal with it intelligently. In the development of our engine and editor, we will have numerous developers  contribute to the code base.  Thus, it is to my benefit to continue this systematic extraction for as long as possible.  I will be able to make it easier for each successive developer to integrate with the code base in less time, an effort that will continue to provide dividends.

Seems like I need to do some more research on refactoring.  And prepare a little bit of a road-map for my colleagues.


2019.09.26
''''''''''

Today again raised questions as to the need for ECS system. With the recent update to the graphic system to include debug drawing, transient components were created to store the debug draw commands.However the transient components were visible within the editor causing a bit of a stir among the designers.

Apparently the idea of all data be encapsulated within components still not intuitive.The notion that some systems must retain data is part of the old component architecture this usually comes from and desire to cashe or preallocate information.  In the true ECS each individual system will have no knowledge of any other system thus there will be no direct communications between systems and the passing of messages must be handled by an unknown entity.To many this seems like an unnecessary step and is often viewed as inefficient.  But the advantage to such an architecture really only shines once the number of systems reaches a critical point.

Before this critical point it's easy to know every system in your architecture and be able to talk with him directly.  With such small architectures it may be possible to change other code to compensate for your your system's implementation.Thus efficiency can be gained because a single person knows both sides.

However, after the critical point is reached the architecture becomes maintained by multiple people generating multitudes of systems. It is often impractical if not impossible for one person to know the entire architecture.As the number of systems grows so does usage of those individual systems. Changing a single system for your architecture may in fact break other systems you are not aware exist. And attempting to update those systems incurs a heavier cost of the developer not even understanding how they work. 

.. Shared systems often trade efficiency for a consistent interface.

In the development of games of very small game could be developed by a group of people where each developer could no significant portion of the game architecture.  Again as the code base grows the proportion of knowledge each developer has of the entire architecture diminishes rapidly.Any form of outsourcing, code reuse from previous projects or employee turnover further decreases knowledgeof all the systems.

Planning your code base for a large architecture adds a bit of overhead your design process; a cost that at the time does not seem valuable.Yet a successful small project will quickly become all very large project as new features are demanded. Under the strain of growth and the expectations that go along with it, re-architecting is almost impossible.Replacing a system interface requires changing all the places that use that interface; which in turn requires retesting all of those pieces. With your success comes new resources in the form of developers who must be trained both in the old architecture and the new architecture so that they can convert from one to the other or build new systems with the new architecture.The existence of two architectures in one code base is a daunting learning curve for any junior developer.


2019.09.27
''''''''''

Today's conundrum comes after reading this article asking about `intersystem communication in an ECS game <https://gamedev.stackexchange.com/questions/100414/intersystem-communication-in-a-ecs-game>`_.  I noticed a small detail in the article that seemed odd to my current methodology: the idea of low-level systems and high-level systems.  In working on reducing coupling of systems in the current engine (or a futile attempt to remove all coupling), I have noticed that there just are some systems that must rely on others.  But, what is the difference between a low-level system and high-level system?

As the article describes some examples, it becomes a little obvious that I'm working only with low-level systems.  Some of the system we have defined are not actually systems: they are functionality.  Actions are not a system, they are a functionality of the engine.  Whatever is causing the creation of Actions is probably a system, and probably a high-level system at that.  Now I'm questioning a lot of what makes a 'system' a system.  Is functionality sufficient?  Is the need to run periodically sufficient?  Is it a system because someone else depends on it?  Is it a system because it depends on no others?

In thinking about the original video which brought the ECS idea to mind, `'Overwatch' Gameplay Architecture and Netcode <https://www.youtube.com/watch?v=W3aieHjyNvw>`_, the systems talked about were already high-level; such as Weapon, Replay, Combat and World.  Nowhere in the list is the systems I've been planning: Event, Messaging, Camera, Persistence, Physics.  So, I now see why I've had such a difficult time separating out my systems.  They aren't systems at all, they are functionality that other systems need to use for their work.  Most can easily be implemented as Singletons or interfaces to libraries.  Most don't need to run on a schedule and should just do their work.  My efforts have been like digging into the bedrock to get the last fragment of dirt; and each shovel just creates more dirt.

I plan to research what other engines tend to use as systems and find a decent designation for our current 'systems'.  'System' needs to be reserved for higher activities.

==========================  ==========================
Systems
------------------------------------------------------
Overwatch (Simple)          Overwatch (Netcode)
==========================  ==========================
TargetName                  NetworkEvent
LifetimeEntity              InstanceClientDebugMessage
PlayerSpawn                 InstanceClientMessage
GameInput                   Observer
Behavior                    Spectator
AimAtTarget                 PathDataInvalidate
MouseCursorFollow           FixedUpdate
ParametricMovement          DerpBot
PlatformerPlayerController  Debug
WallCrawler                 MovementVolume
RaycastMovement             AIStrategic
Physics                     AIPathFind
Grounded                    AIBehavior
Health                      AISpawn
Socket                      AIMovement
Attach                      UnsynchronizedMovement
Camera                      MovementState
DebugEntity                 MovementExertion
ImageAnimation              AIPerception
Render                      Statescript
EntitySpawner               Weapon
LifeSpan                    Anim
SpawnOnDestroy              PvPLegacy
                            Combat
                            World
                            GameMode
                            AIPointFind
                            StatescriptPostSim
                            Predictor
                            Stats
                            Hero
                            SeenBy
                            Voice
                            Mirror
                            Possession
                            NetworkMessage
                            ServerBreakable
                            NetworkFlow
                            InterpolateMovementState
                            SpatialQuery
                            Replay
                            GameModerator
==========================  ==========================

So, as the above table shows, the systems used in Overwatch are high-level.  The low level systems aren't listed, but I can guess at a few of them; namely everything we currently have implemented.  Basically, the systems we have already built are really the underlying framework.  The true systems we just call Scripts.  This explains a lot about why people were having issues adapting.  As they got closer to a system, the less functionality it seemed to have and the less it *needed* to be a system.

To resolve this, I will need to establish a Framework layer upon which all the high-level systems can build.


.. uml::

    @startuml
    top to bottom direction

    /' frame FrameworkLow {
        component ComponentCore
        component ComponentRepository
        component Configuration
        component Events
        component External
        component GameObjectFiltering
        component Messaging
        component Platform
        component SceneManagement
        component SystemManagement
        component LeakDetection
        component Threading
        component EngineInterfaces
        component EngineRunner
        component EventCore
        component Logger
        component Math
        component MathSerialization
        component SerializationCore
        component SystemCore
        component Utility
    }
    frame FrameworkHigh {
        component DynamicWaypoints
        component Actions
        component AudioSystem
        component Input
        component Physics
        component Rendering
        component Scripting
        component AnimationSystem
        component Utility
        component ComponentFactory
        component GamePadSystem
        component MeshDebug
        component Manipulators
        component PersistenceSystem
        component Serialization
        component Graphics.OpenGL4
        component Physics.Bullet
        component ThirdParty
    }
    frame Systems {
        component AudioEngine
        component GameScripts
        component Replay
        component Telemetry
        component Engine
        component ScriptMaps
    }
    frame Executables {
        component Bootstrap
        component Editor
        component AudioTool
        component UnitTests
        component TempestPrebuilder
    }

    component OS

    Bootstrap -[hidden]down-> Systems
    Editor -[hidden]down-> Systems
    AudioTool -[hidden]down-> Systems
    UnitTests -[hidden]down-> Systems
    TempestPrebuilder -[hidden]down-> Systems

    AudioEngine -[hidden]down-> FrameworkHigh
    GameScripts -[hidden]down-> FrameworkHigh
    Replay -[hidden]down-> FrameworkHigh
    Telemetry -[hidden]down-> FrameworkHigh
    Engine -[hidden]down-> FrameworkHigh
    ScriptMaps -[hidden]down-> FrameworkHigh

    DynamicWaypoints -[hidden]down-> FrameworkLow
    Actions -[hidden]down-> FrameworkLow
    AudioSystem -[hidden]down-> FrameworkLow
    Input -[hidden]down-> FrameworkLow
    Physics -[hidden]down-> FrameworkLow
    Rendering -[hidden]down-> FrameworkLow
    Scripting -[hidden]down-> FrameworkLow
    AnimationSystem -[hidden]down-> FrameworkLow
    Utility -[hidden]down-> FrameworkLow
    ComponentFactory -[hidden]down-> FrameworkLow
    GamePadSystem -[hidden]down-> FrameworkLow
    MeshDebug -[hidden]down-> FrameworkLow
    Manipulators -[hidden]down-> FrameworkLow
    PersistenceSystem -[hidden]down-> FrameworkLow
    Serialization -[hidden]down-> FrameworkLow
    Graphics.OpenGL4 -[hidden]down-> FrameworkLow
    Physics.Bullet -[hidden]down-> FrameworkLow
    ThirdParty -[hidden]down-> FrameworkLow

    ComponentCore -[hidden]down-> OS
    ComponentRepository -[hidden]down-> OS
    Configuration -[hidden]down-> OS
    Events -[hidden]down-> OS
    External -[hidden]down-> OS
    GameObjectFiltering -[hidden]down-> OS
    Messaging -[hidden]down-> OS
    Platform -[hidden]down-> OS
    SceneManagement -[hidden]down-> OS
    SystemManagement -[hidden]down-> OS
    LeakDetection -[hidden]down-> OS
    Threading -[hidden]down-> OS
    EngineInterfaces -[hidden]down-> OS
    EngineRunner -[hidden]down-> OS
    EventCore -[hidden]down-> OS
    Logger -[hidden]down-> OS
    Math -[hidden]down-> OS
    MathSerialization -[hidden]down-> OS
    SerializationCore -[hidden]down-> OS
    SystemCore -[hidden]down-> OS
    Utility -[hidden]down-> OS


    Executables -down-> Systems
    Systems -down-> FrameworkHigh
    FrameworkHigh -down-> FrameworkLow
    '/

    frame Executables
    frame Systems
    frame FrameworkHigh
    frame FrameworkLow

    Executables -down-> Systems
    Executables -down-> FrameworkHigh
    Executables -down-> FrameworkLow
    Systems -down-> FrameworkHigh
    Systems -down-> FrameworkLow
    FrameworkHigh -down-> FrameworkLow
    @enduml


This initial diagram may actually be too powerful.  The Executables and the Systems really shouldn't be accessing the low-level APIs directly.  I would amend this diagram to the following:

.. uml::

    @startuml

    frame Executables
    frame Systems
    frame FrameworkHigh
    frame FrameworkLow

    Executables -down-> Systems
    Executables -down-> FrameworkHigh
    Systems -down-> FrameworkHigh
    FrameworkHigh -down-> FrameworkLow

    @enduml


The low-level framework writers now have only one customer and can focus on being efficient.  The high-level framework is where any user-facing API can be implemented and converted.


2019.10.03
''''''''''

Given the simple layers provided from the last post (Executables, Systems, FrameworkHigh, FrameworkLow), the next task to assign existing functionality to the various layers.  Most of our existing functionality is already broken out into modules or folder structures, making this classification task much simpler.

The executable layer is relatively straightforward.  In fact, I sometimes question the need for this layer.  But, I will stick with it until conclusive need excludes it.

.. uml::

    @startuml

    frame Executables {
        component Bootstrap
        component Editor
        component AudioTool
        component UnitTests
        component TempestPrebuilder
    }

    @enduml

The systems layer is a little more complicated.  Currently, the engine doesn't really have anything that constitutes a real system.  None of the existing 'System's are big enough to considered formally.  So, for this discussion, I will lump the Scripts into this category, as they best exemplify the complexity of operation that I would expect a true system to have.

.. uml::

    @startuml

    frame Systems {
        component GameScripts
    }

    @enduml


If all of our existing pieces are not systems, then what are they?  They are Framework modules.  They are significant functionality that is used by the game Systems and provide simplified access to rather complex APIs.  Notice that each module describes the abstract idea being implemented, and excludes listing any specific technologies.  The Framework modules allow the upper Systems to utilize functionality *without* needing to understand the technology.

If you are building some functionality and you create a component for the designer to interact with, then you are probably building a Framework module.

.. uml::

    @startuml

    frame Framework {
        component Actions
        component Input
        component Physics
        component Rendering
        component Animation
        component ComponentRepository
        component GamePad
        component MeshDebug
        component Manipulators
        component Persistence
        component Serialization
        component Audio
        component DynamicWaypoints
        component SystemManagement
        component SceneManagement
    }

    @enduml


And so, the location of the actual technology falls to the last layer, the API layer, previously the FrameworkLow layer.  The modules in this layer are specific to a technology, but are only exposed to the Framework layer and no higher.  Any complexity introduced by an API module is constrained by the Framework layer.  As such, swaping one technology for another is as easy as updating the implementation of a Framework module and nothing more.  

There may be cases where APIs are made available to upper layers as though on level with the Framework layer.  These may occur purely from a convenience argument, in that a Framework module intermediary would be trivial and add no functionality.  This should be done sparingly.

.. uml::

    @startuml

    frame API {
        component SystemCore
        component SerializationCore
        component EngineInterfaces
        component Math
        component Graphics.OpenGL4
        component Physics.Bullet
        component ThirdParty
        component LeakDetection
    }

    @enduml


Here's the final diagram of expected layer interactivity.  The special of API modules is not modeled.


.. uml::

    @startuml

    frame Executables
    frame Systems
    frame Framework as Front {
        frame FrameworkMessaging
        component EngineRunner
        frame ComponentTypes
    }
    frame API {
        component Engine
        component ComponentRepository
    }

    Executables -down-> Systems
    Systems <-down-> Front
    Front <-down-> API

    Executables -left-> EngineRunner
    Systems --> ComponentTypes

    EngineRunner -[hidden]right-> FrameworkMessaging
    FrameworkMessaging -[hidden]right-> ComponentTypes

    Engine -[hidden]right-> ComponentRepository


    @enduml


2019.10.04
''''''''''

Again the question arises of actually classifying existing functionality into the layers above.  Should I even be able to clearly classify each functionality, how should I separate the functionalities?  Should I *even* separate them?

I'm not sure about the correct answer here.  I do know that most of our modules are labelled with 'System' and that is actually misleading many of my developers.  Systems are actually much bigger than what we originally thought.  Many of our modules are tiny and very well defined.  Thus, I will need to make a pass through many of them to rename them, removing the 'System' stigma.

For the sake of convenience, functionality within the Framework and API layers will just be named 'modules'.  I can answer the questions about which layer each module belongs in at a different time.  Changing the name might be enough of a challenge, with a group of developers who have a deeply ingrained doctrine and strong wills against change.  There *will* be crying.  Lots of it.


Changing the name of 'Systems' to 'Modules'
* Determine how to handle module updating and 'frame rates'

    If a module requires a 'frame rate', then there should be a system associated with it.  The system will have the frame rate, the module will not.  Modules that need a frame rate will be severely questioned.  The only two that come to mind are graphics and physics.  Physics requires updating on a fixed time step and must be synchronized with gameplay systems.  Graphics wants to limit its iterations to maintain a consistent time between context flips.  Each of these 'systems' will be trivial, just calling the appropriate framework function at the correct time.

* Update documentation to refer to modules instead of systems

* Update classes named System to be named Module

* Update the SystemManager to be the ModuleManager

* Build a SystemManager for true Systems


Before I can begin the conversion, I want to know how many existing systems are non-trivially using the virtual functions for initialization, update and shutdown.  If I can see how many functions are utilizing the update() method, I might have to change plans.  A system is a class that derived from SystemBase.

======================  ==========  ===========     ======  =========   ========
System                  Initialize  Start Frame     Update  End Frame   Shutdown
======================  ==========  ===========     ======  =========   ========
ActionSystem            .           .               X       .           X
AudioSystem             X           .               X       .           X
DynamicWaypointGraph    .           .               .       .           .
EventSystem             .           .               X       .           .
GamePadSystem           X           X               .       .           .
InputSystem             X           X               .       .           X
MessagingSystem         X           .               X       .           X
PersistenceSystem       .           .               .       .           .
PhysicsSystem           X           X               .       X           X
PlatformWindowing       .           X               .       .           .
CameraSystem            .           .               .       .           .
GraphicsSystem          X           X               X       X           X
ParticleSystem          X           .               X       .           .
Windowing               X           X               .       X           .
InstantReplaySystem     .           .               X       .           .
SceneSystem             .           .               .       .           .
UISystem                .           .               X       .           .
ScriptingSystem         .           .               X       .           .
SystemMonitor           .           .               .       X           .
AnimationSystem         .           X               .       .           .
TelemetrySystem         X           .               X       .           X
======================  ==========  ===========     ======  =========   ========


2019.10.08
''''''''''

Time to finally define a System and a Module.  This distinction will be needed to facilitate the separation of functionality.

:Note: Before beginning, know that may existing Systems and Modules are actually composed of both.  Some will have trivial Systems, but may still be needed.  Having the extra layer is still useful.

A **Module** is a collection of functionality implementing a feature.

A **System** manages a feature over time.

These definitions make a Module and a System not completely mutually exclusive.  The difference really rests with the focus.  Modules may be as simple as APIs, like OpenGL, or more complex like a physics engine.  The module wraps the feature into a set of functions that be used at a higher level.  A module should not have significant amount of data management associated.  More data implies increased complexity, making the module more like a system.

A System handles the complexity of using a feature as time progresses.  The System may be responsible for maintaining data and coordinating use of multiple modules.

Since a System is responsible over time, there is the possibility that multiple systems may be executing.  This will be the case with an engine that supports complex network modelling and prediction.  The Module can assume that there will only be one of them.  If a module wants to create *static* information, it can be assured that all calls to the Module will be accessing that information.  However, for a System, each instance of a System is independent and may be in an entirely different simulation.  Thus, a System should not store information statically as each instance cannot be sure the information pertains to them; it should be using internal variables or components instead.  Components and Systems will be instantiated within the appropriate universe.

Some examples may help identify the differences.

The SceneSystem doesn't have any behavior in the StartFrame, Update or EndFrame functions.  Thus, it doesn't rely on time at all.  The SceneSystem is actually a Module or even an API.  There is some state information created by SceneSystem (obviously the current scene and the game objects), but no behavior that is time related.  I would convert all Scene related information (including Space) into a Module that the engine directly owns.  Thus, the engine can initialize and shutdown the module and should be managing the associated data.  (If this functionality becomes more complex later, a Manager class could be created to contain the functionality).

The GraphicsSystem is actually both and should be split appropriately.  The GraphicsModule, the feature itself, defines functionality for working with the rendering context and window, loading content to the GPU and rendering an object.  However, the GraphicsSystem periodically searches for all renderable objects and demands the GraphicsModule render them.  Additionally, the GraphicsSystem manages the data for the Debug drawing feature to render that periodically too.  This separation takes time away from the features and moves it to the true System, leaving the feature functionality in the Module.  Now, the rendering capability is available to any other System or Module, but they will need to manage their own data and the components utilized by the System is decoupled from the Module.

The InputSystem is an example of a System and Module in one.  One significant issue that is happening within the InputSystem is that InputComponents need to have some data converted.  The internal storage of the key is actually a scancode.  However, the InputComponent itself doesn't have the capability to convert a letter into a scancode.  So, the component is creating a 'message' and storing the message until the next execution of the InputSystem.  The InputSystem, when updating, reads all the messages in an InputComponent and performs the necesssary conversion.  This process is highly circuitous and confusing for developers.  If the InputComponent we able to directly access the module function to perform the conversion, much of the problem could be avoided.  Thus, the InputComponent is actually being created with incomplete data and left in a state that isn't functional, causing the InputSystem to have to perform validation *every frames* to ensure all of the components get updated properly.  The InputSystem does have knowledge of time and will be updating the state of the InputComponents periodically.  However, there is functionality that the InputSystem can delegate to an Input Module which would then become available to everyone, without regard of timing.

The InputSystem in particular would be good for an example of how to break down an over-committed structure into the System-Component-Module layering.


2019.10.21
''''''''''

Initial Structure of Input System

.. uml::

    @startuml
    set namespaceSeparator ::
    class inputComponent {
        +enum Keys
        +enum Cursor
        +inputComponent()
        +~inputComponent()
        {override}+typeRT toTypeRT()
        {override}+updateFromTypeRT(typeRT p_type)
        +bool getButtonState(std::string p_name)
        +float getAxisState(std::string p_name)
        +bool getConnectionState(std::string p_name)
        +std::pair<double, double> getCursorState(std::string p_name)
        +setContext(std::string p_context)
        +std::string getContext()
        +addButtonMapping(unsigned int p_letter, std::string p_name)
        +addButtonMapping(inputComponent::Keys p_key, std::string p_name)
        +removeButtonMapping(std::string p_name)
        +addAxisMapping(int p_axiscode, std::string p_name)
        +removeAxisMapping(std::string p_name)
        +addCursorMapping(Cursor p_cursorcode, std::string p_name)
        +removeCursorMapping(std::string p_name)
        +addGamepadButtonMapping(unsigned int p_button, unsigned int p_id, std::string p_name)
        +removeGamepadButtonMapping(std::string p_name)
        +addGamepadAxisMapping(unsigned int p_axis, unsigned int p_id, std::string p_name)
        +removeGampadAxisMapping(std::string p_name)
        +addControllerStateMapping(unsigned int p_id, std::string p_name)
        +removeControllerStateMapping(std::string p_name)
        +clearAllMaps()
        +lock()
        +unlock()
        {override}+componentType type()
        {static}+componentType getType()
        {static}+componentTypeDetails getTypeDetail()
        -std::mutex m_write_mutex
        -std::string context
        -std::unordered_map<unsigned int, std::string> scancodeToNameMap
        -std::unordered_map<std::string, bool> nameToStatusMap
        -std::unordered_map<int, std::string> axisToNameMap
        -std::unordered_map<std::string, float> nameToRangeMap
        -std::unordered_map<Cursor, std::string> cursorToNameMap
        -std::unordered_map<std::string, std::pair<double, double>> nameToPositionMap
        -std::unordered_map<unsigned int, std::string> gamepadButtonToNameMap
        -std::unordered_map<std::string, actionType> gamepadNameToStatusMap
        -std::unordered_map<std::string, unsigned int> gamepadButtonNameToControllerMap
        -std::unordered_map<unsigned int, std::string> gamepadAxisToNameMap
        -std::unordered_map<std::string, float> gamepadAxisToRangeMap
        -std::unordered_map<std::string, unsigned int> gamepadAxisNameToControllerMap
        -std::map<unsigned int, std::string> controllerToNameMap
        -std::unordered_map<std::string, actionType> controllerNameToStatusMap
        -processRequest(request request)
        -std::queue<inputComponent::request> m_processing_requests
        friend class inputSystem
    }
    class inputSystem {
        +inputSystem(inputSourceInterface * p_input_source_ptr)
        +unsigned int mapLetterToScancode(unsigned int p_letter)
        +addInputSource(inputSourceInterface * p_input_source_ptr)
        {static}+inputSourceInterface * m_base_interface
        {static}+std::string getName()
        {override}+std::string name()
        {static}+bool * getAllKeys()
        {static}+unsigned int MAXIMUM_MOUSE_BUTTONS = 5
        {override}#onInitialize()
        {override}#onStartFrame()
        {override}#onShutdown()
        -bool getKey(int p_key)
        -bool isMouseButton(int p_key)
        -bool getMouseButton(int p_button)
        -actionType getGamepadButton(unsigned int p_button, unsigned int p_id)
        -float getGamepadAxis(unsigned int p_axis, unsigned int p_id)
        -actionType getControllerState(unsigned int p_id)
        -inputSourceInterface * m_input_source_ptr
        -std::list<inputSourceInterface *> m_inputSources
        {static}-inputKeyboardListener(scancodeWrapper p_scancode, actionType p_action, unsigned int p_mods)
        {static}-inputMousePosListener(double p_xpos, double p_ypos)
        {static}-inputGamepadButtonListener(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id)
        {static}-inputGamepadAxisListener(const float * p_axisValues, size_t p_numAxis, unsigned short p_id)
        {static}-inputConnectionStateListener(unsigned short p_id, actionType p_action)
        {static}-int MAX_KEYS = 1024
        {static}-bool[MAX_KEYS] keys
        {static}-std::pair<double, double> mousePosition
        {static}-bool[MAXIMUM_MOUSE_BUTTONS] mouseButtons
        {static}-actionType[MAX_CONTROLLERS][XINPUT_BUTTON_OFFSET_MAX] gamepadButtons
        {static}-float[MAX_CONTROLLERS][gpa_count] gamepadAxis
        {static}-actionType[MAX_CONTROLLERS] gamepadConnection
    }
    class inputSystem::messageCodes {
        {static}+unsigned int convertLetterToScancode = 1
        {static}+unsigned int convertMouseButtonToScancode = 2
    }
    class inputComponent::request {
        +enum req_type
        +unsigned int letter
        +unsigned int mouseButton
        +unsigned int axisCode
        +unsigned int gamepadCode
        +unsigned int gamepadAxisCode
        +unsigned int gamepadID
        +Cursor cursorCode
        +std::string name
        +request(req_type p_type, unsigned int p_letter, unsigned int p_mouseButton, unsigned int p_axiCode, Cursor p_cursorCode, unsigned int p_gamepadButton, unsigned int p_gamepadAxis, unsigned int p_gamepadID, std::string p_name)
        {static}+request makeAddLetterToName(unsigned int p_letter, std::string p_name)
        {static}+request makeAddMouseButtonToName(unsigned int p_mouseButton, std::string p_name)
        {static}+request makeAddAxisToName(int p_axiscode, std::string p_name)
        {static}+request makeAddCursorToName(Cursor p_cursorcode, std::string p_name)
        {static}+request makeRemoveButton(std::string p_name)
        {static}+request makeRemoveAxis(std::string p_name)
        {static}+request makeRemoveCursor(std::string p_name)
        {static}+request makeAddGamepadButton(unsigned int p_gamepadButton, unsigned int p_id, std::string p_name)
        {static}+request makeAddGamepadAxis(unsigned int p_gamepadAxis, unsigned int p_id, std::string p_name)
        {static}+request makeAddControllerState(unsigned int p_id, std::string p_name)
        {static}+request makeRemoveGamepadButton(std::string p_name)
        {static}+request makeRemoveGamepadAxis(std::string p_name)
        {static}+request makeRemoveControllerState(std::string p_name)
    }
    inputComponent -up-|> componentCRTP
    inputSystem -up-|> systemBase
    inputSystem +-- inputSystem::messageCodes
    inputComponent +-- inputComponent::request
    @enduml


The existing structure uses a request message to record requests inside the inputComponent and relies upon the inputSystem to process these messages at a later time in the frame.  While this structure does maintain the separation of data from behavior, it is quite contrived.  The additional overhead of allocation, storage and iteration *per frame* is not optimal and may be more of a hinderance than a benefit.  This is a great example of process being favored over usefulness.

The existing structure also manages inputs from two different categories: Keyboard/Mouse and Gamepads.  These two categories are not significantly different and really should be rolled together, further reducing complexity.  Alas, this optimization isn't necessary at this time and would be better left for a later change.


After InputFramework Extraction

.. uml::

    @startuml
    class inputFramework {
        {static}+attachInputSource(inputSourceInterface * p_input_source_ptr)
        {static}+detachInputSource(inputSourceInterface * p_input_source_ptr)
        {static}+<back:#lightskyblue>bool getKey(scancode p_key)</back>
        {static}+<back:#lightskyblue>float getAxis(unsigned int p_axis)</back>
        {static}+<back:#lightskyblue>bool getMouseButton(MouseButton p_button)</back>
        {static}+<back:#lightskyblue>std::pair<double, double> getMousePosition()</back>
        {static}+<back:#lightgreen>actionType getGamepadButton(unsigned int p_button, gamepad_id p_id)</back>
        {static}+<back:#lightgreen>float getGamepadAxis(unsigned int p_axis, gamepad_id p_id)</back>
        {static}+<back:#lightgreen>actionType getControllerState(gamepad_id p_id)</back>
        {static}-scancode mapLetterToScancode(unsigned int p_letter)
        {static}-<back:#lightskyblue>inputKeyboardListener(scancodeWrapper p_scancode, actionType p_action, unsigned int p_mods)</back>
        {static}-<back:#lightskyblue>inputMousePosListener(double p_xpos, double p_ypos)</back>
        {static}-<back:#lightskyblue>inputMouseButtonListener(unsigned int p_button, actionType p_action, int p_mods)</back>
        {static}-<back:#lightgreen>inputGamepadButtonListener(unsigned short p_gamepadcode, actionType p_action, unsigned short p_id)</back>
        {static}-<back:#lightgreen>inputGamepadAxisListener(float * p_axisValues, size_t p_numAxis, unsigned short p_id)</back>
        {static}-<back:#lightgreen>inputConnectionStateListener(unsigned short p_id, actionType p_action)</back>
        {static}-std::list<inputSourceInterface*> inputSourceInterfaces
        {static}-<back:#lightskyblue>std::map<letter, scancode> letterToScancodeMap</back>
        {static}-<back:#lightskyblue>bool[MAXIMUM_KEY] keys</back>
        {static}-<back:#lightskyblue>float[MAXIMUM_AXIS] axes</back>
        {static}-<back:#lightskyblue>std::pair<double, double> mousePosition</back>
        {static}-<back:#lightskyblue>bool[MAXIMUM_MOUSEBUTTON] mouseButtons</back>
        {static}-<back:#lightgreen>actionType[MAX_CONTROLLERS][XINPUT_BUTTON_OFFSET_MAX] gamepadButtons</back>
        {static}-<back:#lightgreen>float[MAX_CONTROLLERS][gpa_count] gamepadAxis</back>
        {static}-<back:#lightgreen>actionType[MAX_CONTROLLER] gamepadConnection</back>
    }
    class inputSystem {
        +inputSystem(inputSourceInterface * p_input_source_ptr)
        +addInputSource(inputSourceInterface * p_input_source_ptr)
        {static}+std::string getName()
        +std::string name()
        {override}#onInitialize()
        {override}#onShutdown()
        -std::list<inputSourceInterface*> m_inputSources
    }
    class inputComponent{
        +inputComponent()
        +inputComponent(typeRT p_type_data)
        +~inputComponent()
        {override}+typeRT toTypeRT()
        {override}+updateFromTypeRT(typeRT p_type)
        +<back:#lightskyblue>bool getButtonState(name p_name)</back>
        +<back:#lightskyblue>float getAxisState(name p_name)</back>
        +<back:#lightgreen>bool getConnectionState(name p_name)</back>
        +<back:#lightskyblue>cursorPosition getCursorState(name p_name)</back>
        +setContext(std::string p_context)
        +std::string getContext()
        +<back:#lightskyblue>addButtonMapping(name p_name, letter p_letter)</back>
        +<back:#lightskyblue>addButtonMapping(name p_name, Key p_key)</back>
        +<back:#lightskyblue>addButtonMapping(name p_name, MouseButton p_button)</back>
        +<back:#lightskyblue>removeButtonMapping(std::string p_name)</back>
        +<back:#lightskyblue>addAxisMapping(name p_name, axis p_axiscode)</back>
        +<back:#lightskyblue>removeAxisMapping(name p_name)</back>
        +<back:#lightskyblue>addCursorMapping(name p_n ame, Cursor p_cursorcode)</back>
        +<back:#lightskyblue>removeCursorMapping(name p_name)</back>
        +<back:#lightgreen>addGamepadButtonMapping(name p_name, button p_button, gamepad_id p_controllerid)</back>
        +<back:#lightgreen>removeGamepadButtonMapping(name p_name)</back>
        +<back:#lightgreen>addGamepadAxisMapping(name p_name, axis p_axis, gamepad_id p_controllerid)</back>
        +<back:#lightgreen>removeGamepadAxisMapping(name p_name)</back>
        +<back:#lightgreen>addControllerStateMapping(name p_name, gamepad_id p_id)</back>
        +<back:#lightgreen>removeControllerStateMapping(name p_name)</back>
        +clearAllMaps()
        {override}+componentType type()
        {static}+componentType getType()
        {static}+componentTypeDetails getTypeDetail()
        -std::string context
        -<back:#lightskyblue>std::unordered_map<name, letter> nameToLetterMap</back>
        -<back:#lightskyblue>std::unordered_map<name, MouseButton> nameToMouseButtonMap</back>
        -<back:#lightgreen>std::unordered_map<name, gamepad_button> nameToGamepadButtonMap</back>
        -<back:#lightskyblue>std::unordered_map<name, axis> nameToAxisMap</back>
        -<back:#lightgreen>std::unordered_map<name, gamepad_axis> nameToGamepadAxisMap</back>
        -<back:#lightskyblue>std::unordered_map<name, Cursor> nameToCursorMap</back>
        -<back:#lightgreen>std::unordered_map<name, gamepad_id> nameToControllerMap</back>
    }
    class Key <<enum>>
    class MouseButton <<enum>>
    inputComponent -up-|> componentCRTP
    inputSystem -up-|> systemBase
    inputComponent -down-> inputFramework
    inputSystem -down-> inputFramework

    legend
    |= |= Purpose |
    |<back:#lightskyblue>   </back>| Keyboard and Mouse Input |
    |<back:#lightgreen>   </back>| Gamepad Input |
    endlegend
    @enduml


Numerous changes have been rolled into this extraction.  The inputFramework class, being static, handles all inputs within the process.  The long term goal of having multiple simulations running concurrently is maintained by having the inputFramework class being, effectively, a singleton.  Inputs devices are system-wide, not specific to a process; so, it would be easier to handle these devices at a level higher than any simulation or system.

The inputComponent communicates directly with the inputFramework regarding to button, axis and cursor value lookups.  The inputComponent is responsible for the name mappings and the inputFramework is responsible for retaining the current state of the input elements.  The behaviors of the inputComponent are actually just redirects to the inputFramework after mapping the name to a lookup value; acceptable minimal implementation on a component.

The inputSystem, however, has dehydrated significantly.  After the extraction, inputSystem is only responsible for maintaining a list of inputSourceInterfaces that are provided by the engine.  This is a duplication of the list that appears in the inputFramework, but since multiple engines may be executing at the same time, each with their own set of inputSourceInterfaces, the list of sources in the inputSystem is a proper subset of the list maintained by the inputFramework.  Even if only one engine is generally running at a time, duplication of this small list of pointers is ok for our needs.

The inputSystem doesn't implement an onUpdate() function, but implements the onInitialize() and onShutdown() functions to manage the inputSourceInterfaces.  Thus, the inputSystem still does know about time, specifically the start-up and shut-down of the engine.

During the extraction process, I found that the inputMouseButtonListener() callback handler in the inputFramework did not exist.  The other callbacks of the inputSourceInterface were being handled, just not this one.  By adding the function and implementing it, I was able to add mouse button handling to the engine.  I'm not sure how this functionality was removed, but previous to this semester, the games being developed only used the Gamepad controller; so we just never noticed it was missing.


2019.11.05
''''''''''

Another issue has surfaced with a high priority.  The input system doesn't seem to work properly when in the Editor.