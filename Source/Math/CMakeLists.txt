cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(Math STATIC
    Public/MathConstants.hpp
    Public/MathFunctions.hpp Private/MathFunctions.cpp
    #Public/Matrix2x2.h Private/Matrix2x2.cpp # TODO: Fix this!
    Public/Matrix4x4.hpp Private/Matrix4x4.cpp
    Public/Quaternion.hpp Private/Quaternion.cpp
    Public/Shapes.hpp Private/Shapes.cpp
    Public/Vector2.hpp Private/Vector2.cpp
    Public/Vector3.hpp Private/Vector3.cpp
    Public/Vector4.hpp Private/Vector4.cpp
    Public/Color.hpp Private/Color.cpp
    Public/Color4.hpp Private/Color4.cpp
    Public/Aspect.hpp
    Private/Vector.inl
)

# expose headers
target_include_directories(Math
    PUBLIC Public
)

# link to dependencies
target_link_libraries(Math
    PRIVATE GLM
)
