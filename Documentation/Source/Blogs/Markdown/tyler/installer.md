# Creating Installers for Windows and Linux

## Introduction

At DigiPen, the general way in which students create installers for our projects
is through a tool called InnoScript. Usually, students at DigiPen will download
the school-provided sample Inno file, configure it for their project, copy-paste
their completed game into the correct folders, open their Inno IDE and then
compile their installer.

This method works fine for simple use-cases, but there are some problems with
it. First, it is extremely error-prone. Since you are manually copying these
files in it can be very easy to miss a file, which won't end up being caught
until you are testing it or, god-forbid, after you've submitted it. Secondly,
InnoScript is a Windows-only program for producing Windows installers. This
means, of course, that it cannot help at all with creating Linux packages.

With that said, I hope to in this document explain how to create installers in
such a way that Inno is not a hard dependency, and to show how to make
installers for both Linux and Windows. Also, if you haven't yet, I would highly
recommend reading [Cross Platform C++ in 2019](./crossplatformcpp2019.md) first
as I will be assuming some basic knowledge of CMake in this document.

## Linux Packages

I will begin with how to make a Linux package first, as it is the most intuitive
to set up and because I will be re-using some pieces from here to create the
Windows installer in the next section. The first thing we need to do is to
figure out which targets and files we want to install and where. First, let's
say we have the following `CMakeLists.txt`:

```
cmake_minimum_required(VERSION 3.15.0)

project("My Awesome Game")

add_executable(MyAwesomeGame Main.cpp
                             game.cpp
                             enemy.cpp
)
```

Let us also assume that there is a `resources/` folder, which holds various
textures, sounds, and other assets for our game. Finally, we will also assume
that there is a `lib/CommercialEngine.so` file that we need to ship as well.

The first thing we need to do is to tell CMake which files and folders are to be
installed. We do this with the [install](https://cmake.org/cmake/help/v3.13/command/install.html)
command:

```
# ... Rest of the above CMake file

install(TARGETS MyAwesomeGame DESTINATION .)
install(DIRECTORIES resources/ DESTINATION .)
install(FILES lib/CommercialEngine.so DESTINATION .)
```

What we've just done here is specified to CMake that we have 3 different things
that we need to install: The output of `MyAwesomeGame`, the `resources/` folder,
and the `lib/CommercialEngine.so` file. We've also said that these need to go to
the root directory of wherever we end up installing these things to.

Next, we need to set some generic variables for
[CPack](https://cmake.org/cmake/help/latest/module/CPack.html), the CMake module
for generating installers.

```
set(INSTALLER_BASE_DIR ${CMAKE_BINARY_DIR}/Installer)

set(CPACK_PACKAGE_NAME "MyAwesomeGame")
set(CPACK_PACKAGE_VENDOR "DigiPen Institute of Technology")
set(CPACK_PACKAGE_HOMEPAGE_URL "http://www.digipen.edu/")
set(CPACK_RESOURCE_FILE_LICENSE "${INSTALLER_BASE_DIR}/EULA/DigiPen_EULA.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DigiPen")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_CONTACT "youremail@example.com")
set(CPACK_PACKAGE_ICON "${INSTALLER_BASE_DIR}/icons/icon.ico")
set(CPACK_PACKAGE_CHECKSUM)
```

Let's go over what each of these do before moving on.

* `set(INSTALLER_BASE_DIR ${CMAKE_BINARY_DIR}/Installer)`
 - This sets the directory to where the base installer files are. This will
   mainly be used for the Windows installer, but we want to set it here so that
   we can keep some common files in the same place.
* `set(CPACK_PACKAGE_NAME "MyAwesomeGame")`
 - This sets the name of our package to "MyAwesomeGame"
* `set(CPACK_PACKAGE_VENDOR "DigiPen Institute of Technology")`
 - This sets the name of our vendor. While at DigiPen, it will always be this.
* `set(CPACK_PACKAGE_HOMEPAGE_URL "http://www.digipen.edu/")`
 - This is the homepage for this project. In general, this should be left as
   http://www.digipen.edu/
* `set(CPACK_RESOURCE_FILE_LICENSE "${INSTALLER_BASE_DIR}/EULA/DigiPen_EULA.txt")`
 - This is the directory where the License/EULA can be found. You should set
   this to wherever your EULA is located.
* `set(CPACK_PACKAGE_INSTALL_DIRECTORY "DigiPen")`
 - This is the directory in which your program will be installed. Note that
   there is still a prefix for the directory, so your package will be installed
   at `PREFIX/@CPACK_PACKAGE_INSTALL_DIRECTORY@/`
* `set(CPACK_PACKAGE_VERSION_MAJOR "1")`
 - The major version for your package
* `set(CPACK_PACKAGE_VERSION_MINOR "0")`
 - The minor version for your package
* `set(CPACK_PACKAGE_VERSION_PATCH "0")`
 - The patch number for your package
* `set(CPACK_PACKAGE_CONTACT "youremail@example.com")`
 - The contact information for the project. Basically, who should be contacted
   in the event that something goes wrong.
* `set(CPACK_PACKAGE_ICON "${INSTALLER_BASE_DIR}/icons/icon.ico")`
 - The icon for this installer. Only really applies on Windows, and you should
   set this to wherever your installer is located.
* `set(CPACK_PACKAGE_CHECKSUM)`
 - This tells CPack to generate a Checksum of your package. Generally
   recommended, but not required.

Now, those things are going to be required for both Linux and Windows. For the
linux specific settings, we are going to assume that the package you want to
build is for Debian systems, meaning that we want to have the following line:

```
set(CPACK_GENERATOR STGZ;TGZ;TZ;DEB)
```

What this does is tell CPack which installer generators should be used. In this
line, we are including the base three installers (which are tarballs and a
self-extracting tarball), as well as a fourth type, `DEB`, which tells CPack to
generate a `.deb` package. Next, we want to specify where our project's files
should be installed to, which we can do with the following line:

```
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/DigiPen/${CPACK_PACKAGE_NAME}")
```

Now, we need to specify all of the packages that we depend on. This will vary
depending on your project, but generally this should include everything you are
not explicitly including in your package, and can be specified as a
comma-seperated list of every other package your project depends on.

```
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libglew2.0, libfreetype6, libgl1, libc6, libopengl0")
```

Note that this listing uses the
[Debian Package Relationship Syntax](https://www.debian.org/doc/debian-policy/ch-relationships.html),
so look there if you want ideas on more advanced usage of these specifiers.

The next part, which is optional, is that we want to include some maintainer
scripts to our package. These can be useful for showing EULAs to the end-user,
requiring them to accept before continuing. In order to do this, we only need to
specify one more instruction:

```
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${INSTALLER_BASE_DIR}/preinst"
                                       "${INSTALLER_BASE_DIR}/postrm"
                                       # ...
                                       )
```

This adds the `preinst` and `postrm` shell scripts to our package. For
information on what other scripts can be added and how they work, see
[this link](https://wiki.debian.org/MaintainerScripts). Now, all we have to do
to build our installer is the following line:

```
include(CPack)
```

This executes CPack's internal configuration steps, and creates a target for
building the package.

## Windows Packages

This next one is a little bit more tricky. The recommended course of action on
most tutorials is to go through CPack and use the NSIS generator. However, I
found that generator very hard to use and configure, so instead I worked on
being able to use InnoSetup in CMake instead. In order to do this, the first
thing we have to do is to find the InnoSetup compiler:

```
find_program(ISS_COMPILER NAMES iscc ISCC HINTS "C:/Program Files (x86)/Inno Setup 5" "C:/Program Files/Inno Setup 5")
if(NOT ISS_COMPILER)
    message(WARNING "Inno Setup Compiler not found. Will not be able to generate installer.")
else()
    message(STATUS "Using ISS Compiler found at ${ISS_COMPILER}")
```

Once we have found the compiler, we next need to configure the InnoScript. I
took the provided DigiPen InnoScript and modified it so that it can be
configured by CMake, which is done like so:

```
configure_file("${INSTALLER_BASE_DIR}/Installscript.iss.in"
               "${INSTALLER_BASE_DIR}/Installscript.iss"
)
```

This takes the "input" InnoScript and outputs it to a file `InstallScript.iss`.
Next, we need to create a custom target for our installer so that it can be run
later. In order to do this, we use this command:

```
    add_custom_target(installer
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MyAwesomeGame> ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
            COMMAND ${CMAKE_COMMAND} -E copy ${CPACK_PACKAGE_ICON} ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
            COMMAND ${CMAKE_COMMAND} -E copy_directory resources/ ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/resources
            COMMAND ${CMAKE_COMMAND} -E copy lib/CommercialEngine.so ${INSTALLER_BASE_DIR}/GAMEDIRECTORY/
            COMMAND ${ISS_COMPILER} Installscript.iss
            DEPENDS MyAwesomeGame
            WORKING_DIRECTORY ${INSTALLER_BASE_DIR})
```

What this command does is create a new compilable target called `installer`
which, when run, will first copy all files for our project into the directories
that DigiPen's InstallScript works with, before running the InnoSetup compiler
on our configured `Installscript.iss` in the `${INSTALLER_BASE_DIR}` directory,
while also mentioning that it depends on MyAwesomeGame being compiled first.

## Conclusion

And with that, we have working installers for both Linux and Windows, using
Debian packages and InnoSetup respectively. Do note that there are better ways
to handle some of these. For the Linux packages, the maintainer scripts should
really use tools like `debconf` for displaying the EULA, whereas currently the
ones I've written simply print it to the terminal and wait for either a Y or an
N to be pressed. For the Windows Installer, the InnoSetup script
could probably be made more efficient, requiring less copies to be made by
instead configuring InnoSetup to search for all of the files in their respective
directories. Additionally, the Windows Installer could probably access the list
of `install` commands like CPack does so that they don't have to be specified
manually in the `add_custom_target` command.

