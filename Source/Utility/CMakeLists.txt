cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(Utility STATIC
    Public/DataTypes.hpp Private/DataTypes.cpp
    Public/Enumerate.hpp Public/Enumerate.inl
)

# expose headers
target_include_directories(Utility
    PUBLIC Public
)

if(UNIX)
    target_link_libraries(Utility
        PRIVATE uuid
    )
endif()


# link to dependencies
#target_link_libraries(Utility
#    PRIVATE SerializationCore
#)

set_target_properties(Utility PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)
