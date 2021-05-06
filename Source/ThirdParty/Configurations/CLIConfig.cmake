find_path(CLI_ROOT
    NAMES include/CLI/CLI.hpp
    PATH_SUFFIXES CLI11-
)

find_path(CLI_INCLUDE_DIRS
    NAMES CLI/CLI.hpp
    PATHS ${CLI_ROOT}
    PATH_SUFFIXES include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CLI
    FOUND_VAR
        CLI_FOUND
    REQUIRED_VARS
        CLI_INCLUDE_DIRS
)

if(CLI_FOUND)
    if(NOT TARGET CLI)
        add_library(CLI INTERFACE)
        target_include_directories(CLI INTERFACE ${CLI_INCLUDE_DIRS})
    endif(NOT TARGET CLI)
endif(CLI_FOUND)