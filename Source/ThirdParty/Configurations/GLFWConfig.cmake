find_path(ThirdParty_ROOT
    NAMES Configurations/GLFWConfig.cmake
)

if(NOT TARGET GLFW)
    configure_file(${ThirdParty_ROOT}/Configurations/glfw-3.in ${CMAKE_BINARY_DIR}/glfw-download/CMakeLists.txt)

    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/glfw-download"
        RESULT_VARIABLE exec_res_1)

    execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/glfw-download"
        RESULT_VARIABLE exec_res_2 )

    # prevent building projects we don't want/need
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
    set(GLFW_VULKAN_STATIC OFF CACHE BOOL "")
    set(GLFW_INSTALL OFF CACHE BOOL "")

    file(TOUCH "${CMAKE_BINARY_DIR}/glfw-src/include/GLFW/.customdlglfw")

    # add glfw directly to our build
    add_subdirectory("${CMAKE_BINARY_DIR}/glfw-src" "${CMAKE_BINARY_DIR}/glfw-build")

    # organize the glfw projects in the folder view
    set_target_properties(glfw PROPERTIES FOLDER "ThirdParty/GLFW")

    # Force the includes into the targets since glfw doesn't do that for us.
    #  They expect us to install glfw not build it directly.
    target_include_directories(glfw PUBLIC ${CMAKE_BINARY_DIR}/glfw-src/include/GLFW)
endif()

set(GLFW_ROOT "${CMAKE_BINARY_DIR}/glfw-src")

set(GLFW_INCLUDE_DIRS "${GLFW_ROOT}/include" "${GLFW_ROOT}/include/GLFW")

message(STATUS "GLFW_ROOT=${GLFW_ROOT}")
message(STATUS "GLFW_INCLUDE_DIRS=${GLFW_INCLUDE_DIRS}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW
    FOUND_VAR
        GLFW_FOUND
    REQUIRED_VARS
        GLFW_INCLUDE_DIRS
        # GLFW_LIBRARIES
)

if(GLFW_FOUND)
    if(NOT TARGET GLFW)
        add_library(GLFW INTERFACE)
        target_include_directories(GLFW INTERFACE ${GLFW_INCLUDE_DIRS})
		if(UNIX)
	        target_link_libraries(GLFW INTERFACE glfw3)
		else()
			target_link_libraries(GLFW INTERFACE glfw)
		endif()
    endif(NOT TARGET GLFW)
endif(GLFW_FOUND)


