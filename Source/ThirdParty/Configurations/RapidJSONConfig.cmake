find_path(RapidJSON_ROOT
    NAMES rapidjson.h
    PATH_SUFFIXES rapidjson-
)

find_path(RapidJSON_INCLUDE_DIRS
    NAMES rapidjson.h
    PATHS ${RapidJSON_ROOT}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RapidJSON
    FOUND_VAR
        RapidJSON_FOUND
    REQUIRED_VARS
        RapidJSON_INCLUDE_DIRS
)

if(RapidJSON_FOUND)
    if(NOT TARGET RapidJSON)
        add_library(RapidJSON INTERFACE)
        target_include_directories(RapidJSON INTERFACE ${RapidJSON_INCLUDE_DIRS})
    endif(NOT TARGET RapidJSON)
endif(RapidJSON_FOUND)
