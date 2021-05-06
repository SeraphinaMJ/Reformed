cmake_minimum_required(VERSION 3.15.0)

#find_package(BulletInternal REQUIRED)


add_library(Physics.Bullet STATIC
    BulletWrapper.hpp BulletWrapper.cpp
    DLLControl.hpp
    PhysicsInterface.hpp PhysicsInterface.cpp
    )

if(UNIX)
    find_package(LinuxWinDefs REQUIRED)
endif()

target_include_directories(Physics.Bullet
#    PRIVATE BulletInternal
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${LinuxWinDefs_INCLUDES}
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    # TODO: Remove these references \/ \/ \/
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/TempestEngine
)

target_link_libraries(Physics.Bullet
    PUBLIC BulletCollision
    PUBLIC BulletDynamics
    PUBLIC LinearMath
    PRIVATE Logger
    PRIVATE Configuration
    PRIVATE Math
#    PRIVATE BulletInternal
    PRIVATE ComponentCore
    PRIVATE SerializationCore
    PRIVATE EventSystem
)

set_target_properties(Physics.Bullet PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)
