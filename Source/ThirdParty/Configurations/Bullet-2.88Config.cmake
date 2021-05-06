find_path(ThirdParty_ROOT
    NAMES Configurations/Bullet-2.88Config.cmake
)


# download and unpack bullet at configuration time
configure_file(${ThirdParty_ROOT}/Configurations/Bullet-2.88.in ${CMAKE_BINARY_DIR}/Bullet-download/CMakeLists.txt)

set(INSTALL_LIBS OFF)
set(INSTALL_CMAKE_FILES OFF)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Bullet-download"
    RESULT_VARIABLE exec_res_1)

execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Bullet-download"
    RESULT_VARIABLE exec_res_2 )

# prevent building projects we don't want/need
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "")
set(BUILD_BULLET3 OFF CACHE BOOL "")
set(BUILD_CLSOCKET OFF CACHE BOOL "")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "")
set(BUILD_ENET OFF CACHE BOOL "")
set(BUILD_EXTRAS OFF CACHE BOOL "")
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "")
set(INSTALL_CMAKE_FILES OFF CACHE BOOL "")

set(INSTALL_LIBS OFF CACHE BOOL "Include Bullet in the installation process")
set(INSTALL_CMAKE_FILES OFF CACHE BOOL "Include Bullet .cmake files in the installation process.")
# add bullet directly to our build
add_subdirectory("${CMAKE_BINARY_DIR}/Bullet-src" "${CMAKE_BINARY_DIR}/Bullet-build")

# organize the Bullet projects in the folder view
set_target_properties(
    Bullet3Common
    BulletCollision
    BulletDynamics
    BulletInverseDynamics
    BulletSoftBody
    LinearMath
    PROPERTIES FOLDER "ThirdParty/Bullet"
)

# Force the includes into the targets since Bullet doesn't do that for us.
#  They expect us to install Bullet not build it directly.
target_include_directories(BulletCollision SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/Bullet-src/src)
target_include_directories(BulletDynamics SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/Bullet-src/src)
target_include_directories(BulletInverseDynamics SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/Bullet-src/src)
target_include_directories(BulletSoftBody SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/Bullet-src/src)
