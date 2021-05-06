
find_path(GLM_ROOT
    NAMES glm/glm/glm.hpp
    PATH_SUFFIXES glm-0.9.8.5
)

find_path(GLM_INCLUDE_DIRS
    NAMES glm/glm.hpp
    PATHS ${GLM_ROOT}
    PATH_SUFFIXES glm
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM
    FOUND_VAR
        GLM_FOUND
    REQUIRED_VARS
        GLM_INCLUDE_DIRS
)

if(GLM_FOUND)
    if(NOT TARGET GLM)
        add_library(GLM INTERFACE)
        target_include_directories(GLM INTERFACE ${GLM_INCLUDE_DIRS})
    endif(NOT TARGET GLM)
endif(GLM_FOUND)