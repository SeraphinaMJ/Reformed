cmake_minimum_required(VERSION 3.15.0)

project(SupremeToyInc)

# setup policies
cmake_policy(SET CMP0091 NEW) # allow setting of the MSVC runtime library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # statically linked Debug/Release

# Specify the available configurations (default has a few more)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Configurations
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty
    ${CMAKE_PREFIX_PATH}
    )
set(CMAKE_MODULE_PATH Source/Modules ${CMAKE_MODULE_PATH})

# Download Dependencies
find_package(Bullet-2.88 REQUIRED)

# Enable the use of folders in the solution and file structures in IDE
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set all projects to output to the same directory
# Changing this directory will require updating the default path inside Bootstrap.
#   These are synchronized to make debugging the application within VS easier.
if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP8")
else()
    # On Linux, executables don't get extensions, which causes a problem when
    #  the target is written to the same directory as the build files
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Runtime)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Runtime)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Runtime)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec -fms-extensions -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-private-field -Wno-unused-const-variable -Wno-char-subscripts -Wno-unused-function -Wno-microsoft-extra-qualification -Wno-unused-parameter -Wno-missing-field-initializers")

    # This is so that the program will look in . first for any dynamic libraries
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath='$ORIGIN'")
endif()

set(PROGRAMS_BASE_DIR ${CMAKE_BINARY_DIR}/../Programs)
set(INSTALLER_BASE_DIR ${CMAKE_BINARY_DIR}/../Installer)

set(CMAKE_CXX_STANDARD 17)
link_directories("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Bring in all modules
# #######################################################
#                BEGIN MODULES REGISTRY
# #######################################################
add_subdirectory(Logger)
add_subdirectory(Math)
add_subdirectory(Configuration)
add_subdirectory(Utility)
add_subdirectory(SerializationCore)
add_subdirectory(MathSerialization)
add_subdirectory(EngineInterfaces)
add_subdirectory(ComponentCore)
add_subdirectory(SystemCore)
add_subdirectory(ComponentRepository)
add_subdirectory(EngineRunner)
add_subdirectory(EventSystem)
add_subdirectory(InputFramework)
add_subdirectory(EditorAdapter)
# #######################################################
#                 END MODULES REGISTRY
# #######################################################

# Bring in Audio engine and tool
add_subdirectory(Engine/AudioEngine)
add_subdirectory(Engine/AudioTool)

# Bring in graphics, physics and the engine
add_subdirectory(Graphics.OpenGL4)
add_subdirectory(Physics.Bullet)
add_subdirectory(Engine/TempestEngine)

# Bring in the google test functionality
set(BUILD_GTEST ON CACHE BOOL "")
set(BUILD_GMOCK OFF CACHE BOOL "")
set(INSTALL_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")
add_subdirectory(ThirdParty/googletest-master)

set(BOOTSTRAP_OUTPUT_NAME "ReformedDemo")
set(GAME_DESCRIPTION "Minion controlling tower defense game. Make sure to expand the details window after installation has begun and accept the EULA!")

# Bring in the bootstrap, editor and unittests
add_subdirectory(Bootstrap)
add_subdirectory(Editor)
add_subdirectory(Engine/UnitTests)

# Bring in the TempestPrebuilder if requested
option(INCLUDE_TEMPEST_PREBUILDER "Include the Tempest Prebuilder project and its dependencies (very large)" OFF)
if(INCLUDE_TEMPEST_PREBUILDER)
    add_subdirectory(TempestPrebuilder)
endif(INCLUDE_TEMPEST_PREBUILDER)

# Organize the targets into folders
set_target_properties(
    ImGui
    ImGui_OpenGL3_GLFW_GLEW
    ImGui_Stdlib
    ImGuizmo
    gtest
    gtest_main
    PROPERTIES FOLDER "ThirdParty"
)

# Set up all information on how to install the project.
# #######################################################
#               BEGIN INSTALLATION REGISTRY
# #######################################################

set(CPACK_PACKAGE_NAME ${BOOTSTRAP_OUTPUT_NAME})
set(CPACK_PACKAGE_VENDOR "DigiPen Institute of Technology")
set(CPACK_PACKAGE_HOMEPAGE_URL "http://www.digipen.edu/")
set(CPACK_RESOURCE_FILE_LICENSE "${INSTALLER_BASE_DIR}/EULA/DigiPen_EULA.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DigiPen")

set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_CONTACT "tyler.robbins@digipen.edu") # TODO
set(CPACK_PACKAGE_ICON "${INSTALLER_BASE_DIR}/icons/icon.ico")
set(CPACK_PACKAGE_CHECKSUM)

if(WIN32)
    set(INSTALL_DESTINATION .)
else()
    set(INSTALL_DESTINATION "opt/DigiPen/${CPACK_PACKAGE_NAME}")
endif()

install(TARGETS Bootstrap DESTINATION ${INSTALL_DESTINATION})
install(TARGETS Editor DESTINATION ${INSTALL_DESTINATION})
# install(TARGETS AudioTool DESTINATION ${INSTALL_DESTINATION})
if(WIN32)
    set(DEFAULT_EXECUTABLE "${BOOTSTRAP_OUTPUT_NAME}.exe")
    install(FILES ${TempestEngine_RESOURCES} DESTINATION ${INSTALL_DESTINATION})
else()
    set(DEFAULT_EXECUTABLE "${BOOTSTRAP_OUTPUT_NAME}")
    install(FILES ${FBX_RESOURCES} DESTINATION ${INSTALL_DESTINATION})
endif()
install(DIRECTORY ../Samples/Data DESTINATION ${INSTALL_DESTINATION})
install(DIRECTORY ../Samples/Assets DESTINATION ${INSTALL_DESTINATION})
install(FILES ${CPACK_PACKAGE_ICON} DESTINATION ${INSTALL_DESTINATION})

include(InstallRequiredSystemLibraries)

# set(CPACK_PACKAGE_EXECUTABLES "Bootstrap;TempestEngine")

# If we are windows, use NSIS to generate a windows installer
if(WIN32)
    set(CPACK_NSIS_COMPRESSOR lzma)
    set(CPACK_NSIS_MUI_ICON "${CPACK_PACKAGE_ICON}")


    find_program(ISS_COMPILER NAMES iscc ISCC HINTS "C:/Program Files (x86)/Inno Setup 5" "C:/Program Files/Inno Setup 5")
    # set(ISS_COMPILER "${CMAKE_BINARY_DIR}/../InnoSetup5/ISCC.exe")

    if(NOT ISS_COMPILER)
        message(WARNING "Inno Setup Compiler not found. Will not be able to generate installer.")
    else()
        message(STATUS "Using ISS Compiler found at ${ISS_COMPILER}")

        configure_file("${INSTALLER_BASE_DIR}/Installscript.iss.in"
                       "${INSTALLER_BASE_DIR}/Installscript.iss"
        )

        # string(REPLACE "/" "\\" WIN32_INSTALLER_BASE_DIR ${INSTALLER_BASE_DIR})
        message(STATUS "${WIN32_INSTALLER_BASE_DIR}\\Installscript.iss")
        add_custom_target(installer
			# First make sure that the previous files are removed
			COMMAND ${CMAKE_COMMAND} -E echo "Removing previous files."
			COMMAND ${CMAKE_COMMAND} -E remove_directory ${INSTALLER_BASE_DIR}/GAMEDIRECTORY
			COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALLER_BASE_DIR}/GAMEDIRECTORY
			
			COMMAND ${CMAKE_COMMAND} -E echo "Copying all files over to be installed."
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Bootstrap> ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Editor> ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
            # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:AudioTool> ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
			COMMAND ${CMAKE_COMMAND} -E copy ${CPACK_PACKAGE_ICON} ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
            COMMAND ${CMAKE_COMMAND} -E copy_directory ../Samples/Data ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/Data
            COMMAND ${CMAKE_COMMAND} -E copy_directory ../Samples/Assets ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/Assets
            COMMAND ${CMAKE_COMMAND} -E copy ${TempestEngine_RESOURCES} ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
			
			COMMAND ${CMAKE_COMMAND} -E echo "Running InnoSetup on Installscript"
            COMMAND ${ISS_COMPILER} Installscript.iss
            DEPENDS Bootstrap Editor # AudioTool
            WORKING_DIRECTORY ${INSTALLER_BASE_DIR})
    endif(NOT ISS_COMPILER)
else()
    set(CPACK_GENERATOR STGZ;TGZ;TZ;DEB)
    set(CPACK_PACKAGING_INSTALL_PREFIX "/")

    set(CPACK_DEBIAN_PACKAGE_PREDEPENDS "debconf (>= 0.5) | debconf-2.0")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libuuid1, libportaudio2, libglew2.0, libfreetype6, libxcb1, libgl1, libc6, libasound2, libpng16-16, libopengl0")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${GAME_DESCRIPTION})

    file(READ ${CPACK_RESOURCE_FILE_LICENSE} EULA_CONTENTS)
    string(REPLACE "\n" "\n " EULA_CONTENTS ${EULA_CONTENTS})

    configure_file("${PROGRAMS_BASE_DIR}/preinst.in" "${PROGRAMS_BASE_DIR}/preinst")
    configure_file("${PROGRAMS_BASE_DIR}/Template.desktop.in" "${PROGRAMS_BASE_DIR}/${CPACK_PACKAGE_NAME}.desktop")
    # configure_file("${PROGRAMS_BASE_DIR}/templates.in" "${PROGRAMS_BASE_DIR}/templates")

    install(FILES ${PROGRAMS_BASE_DIR}/${CPACK_PACKAGE_NAME}.desktop DESTINATION usr/share/applications/)

    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROGRAMS_BASE_DIR}/preinst"
        # "${PROGRAMS_BASE_DIR}/templates"
                                           "${PROGRAMS_BASE_DIR}/postrm")

    include(CPack)
endif()

