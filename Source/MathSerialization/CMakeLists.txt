cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(MathSerialization STATIC
    Public/QuaternionSerialization.hpp Private/QuaternionSerialization.cpp
    Public/Vector2Serialization.hpp Private/Vector2Serialization.cpp
    Public/Vector3Serialization.hpp Private/Vector3Serialization.cpp
    Public/Vector4Serialization.hpp Private/Vector4Serialization.cpp
    Public/ColorSerialization.hpp Private/ColorSerialization.cpp
    Public/Color4Serialization.hpp Private/Color4Serialization.cpp
)

# expose headers
target_include_directories(MathSerialization
    PUBLIC Public
)

# link to dependencies
target_link_libraries(MathSerialization
    PRIVATE Math
    PRIVATE SerializationCore
)
