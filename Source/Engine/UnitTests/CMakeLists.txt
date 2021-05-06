cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../Programs/cmake-precompiled-header")
include(PrecompiledHeader)

find_package(GLM REQUIRED)

add_executable(UnitTests
    Precompiled.cpp # this MUST be the first entry in the files list (for cotire)
    Precompiled.h
    ComponentTypeSetTests.cpp
    DataAssuranceTests.cpp
    GameObjectTests.cpp
    MathFunctionsTest.cpp
    Matrix4x4Tests.cpp
    PrintHelpers.hpp PrintHelpers.cpp
    QuaternionTest.cpp
    TransformTests.cpp
    Vector2Tests.cpp
    Vector3Tests.cpp
    Vector4Tests.cpp
    EventSystemTests.cpp
    ZipTests.cpp
    )

target_link_libraries(UnitTests
    PRIVATE gtest
    PRIVATE gtest_main
    PRIVATE GLM
    PRIVATE Math
    PRIVATE MathSerialization
    PRIVATE TempestEngine
    PRIVATE BulletDynamics
    PRIVATE EventSystem
    PRIVATE ComponentRepository
)

# build up resources list
list(APPEND resources ${TempestEngine_RESOURCES})
set(UnitTests_RESOURCES ${resources} CACHE STRING "UnitTests_RESOURCES" FORCE)

# deploy resource files
foreach(resource_file ${UnitTests_RESOURCES})
    add_custom_command(
        TARGET UnitTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${resource_file}
            $<TARGET_FILE_DIR:UnitTests>
    )
endforeach(resource_file)

# setup the precompiled header
add_precompiled_header(UnitTests Precompiled.h
    SOURCE_CXX Precompiled.cpp FORCEINCLUDE)
