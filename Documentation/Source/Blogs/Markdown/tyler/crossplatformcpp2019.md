# Cross Platform C++ in 2019

## Introduction

I've spent many hours over the past two weeks trying to make a Windows-only
project configure, compile, link, and ultimately run on Linux. In the process,
I was able to encounter and solve many of the issues that people would end up
similarly running into as well.

## The Buildsystem

When striving to create cross platform code, it is important to have a well-
crafted build system which can be used generate the project files/Makefiles that
are used to compile and link the rest of the project. Such tools are relatively
easy to use and implement, such as [CMake](https://cmake.org/) and
[PreMake](https://premake.github.io/), which take in simple scripts that
precisely tell the programs how the project files and Makefiles should be
generated. An example of a simple CMake script would look as follows:

```
cmake_minimum_required(VERSION 3.15.0)

project("My Simple Project")

add_executable(simple main.cpp
                      test.cpp
)
```

and a PreMake script:

```lua
workspace "MySimpleProject"
    configurations { "Debug", "Release" }

project "simple"
    kind "ConsoleApp"
    language "C++"

    files { "main.cpp", "test.cpp" }
```

With just scripts like these, as well as the software necessary to interpret
them, it is possible to generate robust project files no matter which operating
system you are actually on. Additionally, both tools have many more advanced
options that can allow more finely tuned control over the specific build options
and configuration of one's project.

However, even with tools such as this, it is possible to accidentally write your
scripts in such a way that they will only work on one operating system. This is
the problem I ran into when I first started the task of making the Tempest
Engine run on a Linux machine. The first issue that came up was with third party
libraries. CMake, which is the tool used for this particular project, has
various functions built-in for finding files, directories, and libraries. This
is useful for when you are using a modified version of a library, or a library
which does not ship its own custom CMake configuration files. However, if you
only ever search for the Windows versions of these libraries, then you
completely ignore one of the main advantages of using CMake in the first place.

Take the `FBXConfig.cmake` file from the Tempest Engine:

```cmake
find_path(FBX_ROOT
    NAMES include/fbxsdk.h
    PATH_SUFFIXES fbx-
)

find_path(FBX_INCLUDE_DIRS
    NAMES fbxsdk.h
    PATHS ${FBX_ROOT}
    PATH_SUFFIXES include
)

find_library(FBX_LIBRARIES
    NAMES
        libfbxsdk
        libfbxsdk-mt
        libfbxsdk-md
    PATHS ${FBX_ROOT}
    PATH_SUFFIXES lib/vs2015/x64/release
)

find_file(FBX_RESOURCES
    NAMES libfbxsdk.dll
    PATHS ${FBX_ROOT}
    PATH_SUFFIXES lib/vs2015/x64/release
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FBX
    FOUND_VAR
        FBX_FOUND
    REQUIRED_VARS
        FBX_INCLUDE_DIRS
        FBX_LIBRARIES
        FBX_RESOURCES
)

if(FBX_FOUND)
    if(NOT TARGET FBX)
        add_library(FBX INTERFACE)
        target_include_directories(FBX INTERFACE ${FBX_INCLUDE_DIRS})
        target_link_libraries(FBX INTERFACE ${FBX_LIBRARIES})
    endif(NOT TARGET FBX)
endif(FBX_FOUND)
```

Because this file only ever searches for the `libfbxsdk.dll`, `libfbxsdk.lib`,
`libfbxsdk-mt.lib`, and `libfbxsdk-md.lib` libraries, this file does not
end up configuring on Linux machines, despite the fact that FBX is available on
the system. When encountering issues like this, it is not very easy to
generalize a solution, as both Windows and Linux treat dynamic libraries
differently, with Windows requiring both the `.dll` file as well as an
associated `.lib`, and Linux only requiring the `.so` file. If there is no way
to generalize this, then the best solution is to simply wrap the smallest amount
of the file possible in an OS check like so:

```cmake
if(WIN32)
    find_library(FBX_LIBRARIES
        NAMES
            libfbxsdk
            libfbxsdk-mt
            libfbxsdk-md
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/vs2015/x64/release
    )

    find_file(FBX_RESOURCES
        NAMES libfbxsdk.dll
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/vs2015/x64/release
    )
else()
    find_library(FBX_LIBRARIES
        NAMES
            fbxsdk
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/gcc4/x64/release
    )

    find_file(FBX_RESOURCES
        NAMES libfbxsdk.so
        PATHS ${FBX_ROOT}
        PATH_SUFFIXES lib/gcc4/x64/release
    )
endif()
```

By doing this, we allow for Linux specific library searching, as well as
maintaining complete compatibility with existing code, by only modifying the
parts of the file that fails to configure on Linux machines. Depending on the
size of the project and the number of dependencies, there could be any number of
issues similarly to this, however all of them can generally be solved in the
same way.

One last note to be wary of with CMake in particular, as it came up while I was
porting this project in the early stages, is to be careful about implicit
inclusions and linkages. This mainly comes up when porting from Windows, or more
specifically from MSVC, to anything else as many utility libraries are linked
against implicitly, such as dynamic library loading and threading libraries.
Unlike on Windows, these capabilities are generally not implicitly linked into
the project for you on Linux. This simply means that if you get strange linker
errors about libraries you don't think you are using, make sure you link against
the correct system libraries, the main culprits of which being `dl` and
`pthread`.

## The Compilation Process

Once the project is completely configured, the next stage is to make sure it can
compile properly. The difficulty of this task can vary from project to project,
and from team to team. The important thing to keep in mind is that compilers on
Linux, such as GCC and Clang, tend to be more strict about what code is
acceptible per the C/C++ Standard than MSVC is. Most of these issues GCC and
Clang will simply warn about, however certain statements such as:

```cpp
return sizeof variable;
```

Will result in a compilation error under Clang, but will pass MSVC perfectly
fine. When it comes to dealing with these errors, it is important to remember
that Clang and GCC will adhere to the C/C++ standard far more rigorously than
MSVC does, so in order to solve these properly, is to simply try to modify the
code so that it follows the standard above all else, regardless of whether or
not MSVC says that a given snippet of code is okay. Additionally, it is
recommended, though not required, to fix as many warnings as possible, as Clang
and GCC will warn for almost anything that the compilers may consider to be
undefined or otherwise dangerous behavior whereas MSVC does not. This can be
very useful in tracking down bizarre runtime errors and bugs at compile time
that could cause major issues later down the line.

### Microsoft Extensions

As a sub-category to overall project compilation, it is important to also
discuss the various extensions that Microsoft has added to C/C++ which are
available by default when compiling with MSVC. As mentioned previously, MSVC has
a tendency to not follow the standard as much as Clang and GCC do. This means
that it makes available, and in some cases encourages, the use of MSVC-specific
functions, variables, and macros. In general, these only provide minor
conveniences to the programmer, but don't usually provide much benefit over the
standard C/C++ functions. Since these are non-standard, GCC and Clang do not
currently implement them, and these functions will not compile properly when
using them. Therefore, there are really only three possible options when dealing
with this. The first, and the one I personally recommend, is to simply avoid
using these extensions at all, as they provide very little real benefit and
can actively make programming for other systems harder. However, some people
actively enjoy using them, for the benefits they can have, so in that case you
can also try to re-implement them. This isn't too difficult, as the functions
themselves are usually simple wrappers built on top of existing standard
functionality. The third and final method, which I highly recommend against
unless there is no real other way of doing so, is to use the MSVC `_WIN32` macro
to conditionally compile different code for Windows and Linux. This should be
avoided however except in circumstances where there is no real way around it,
such as an example I ended up running into with the MSVC extension of `WinMain`
in main files. In those cases, there is no real way around having to have
different code on both Linux and Windows, and you end up needing to do something
like the following:

```cpp
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
```

## System Specific Libraries

Another issue that will inevitably come up when trying to be cross platform is
the problem of system-specific libraries. As a general rule of thumb, these
should just be avoided in favor of libraries that are already cross platform,
as they generally will work just as well as the system specific libraries. For
example, using open, cross-platform, standards such as OpenGL for graphics,
rather than DirectX, or using libraries such as SDL or GLFW instead of
interfacing with the Windows API or with X.Org manually will not only allow for
being cross-platform, but it also reduces the amount of code that needs to be
duplicated per system.

However, cross-platform libraries can't always be used. For one reason or
another, the cross-platform libraries may not be useable. Perhaps it is a
licensing issue, or the library is too bloated when only one or two pieces of
functionality are actually needed, or maybe the codebase is already written
using a system specific library and it would be too costly to rewrite all of the
parts that depend on it. Regardless of the reason, these situations can and do
arise all the time. For this problem, there are two solutions. The first is to
simply write the module of code twice, once using a library for one system,
and once using a different library for the other system. This has obvious
problems, as different libraries may be implemented differently and it runs the
problem that you have to effectively write the same code in different ways for
each system you want to support. An alternative solution, is to write your code
using one system-specific library once. Then, re-implementing that library in a
minimal fashion using the utilities/libraries for the other systems that are
supported. While still not as good as just using one library that works
everywhere, this is the next best thing, as you are turning that system-specific
library into your own library that _can_ in fact, run everywhere.

I ran into this problem myself when working on this originally Windows-only
project specifically with the XInput library. XInput, if you weren't aware, is a
Windows-only library for gamepad/controller support. Since it is Windows-only,
it wouldn't run on Linux out of the box, and I decided to try re-implementing it
using internal Linux functionality as opposed to trying to bring in additional
library dependencies. The way I ended up doing this was to write an minimal
`xinput.h` file that perfectly (or near-enough, I used `enum`s instead of
preprocessor macros for the various constants internally) mimics the one used on
Windows. Then, I wrote a new `XInput.c` file which implements the functions
declared in the new `xinput.h`. Additionally, because only two functions from
XInput are ever actually used in the code-base, I only needed to re-implement
those specific functions myself:

```c
#ifdef __cplusplus
extern "C" {
#endif

// void XInputEnable(BOOL);
DWORD XInputSetState(DWORD, XINPUT_VIBRATION*);
DWORD XInputGetState(DWORD, XINPUT_STATE*);
// DWORD XInputGetKeystroke(DWORD, DWORD, PXINPUT_KEYSTROKE);
// DWORD XInputGetCapabilities(DWORD, DWORD, XINPUT_CAPABILITIES*);
// DWORD XInputGetDSoundAudioDeviceGuids(DWORD, GUID*, GUID*);
// DWORD XInputGetBatteryInformation(DWORD, BYTE, XINPUT_BATTERY_INFORMATION*);

#ifdef __cplusplus
}
#endif
```

The implementation of these functions mostly follows the information that can be
found at [this link](https://www.kernel.org/doc/Documentation/input/joystick-api.txt)
as well as information on Microsoft's documentation on how they are expected to
work. Internally, these functions simply use native linux library calls, such
as `ioctl`, `read`, `write`, `errno`, and `syslog`, while keeping forward facing
types consistent with the types used on Windows, such as `DWORD`, as can be seen
in this example:

```c
static int addDevice(DWORD p_device_id) {
    // bad device id
    if(p_device_id > XUSER_MAX_COUNT) {
        syslog(LOG_WARNING, "Device ID %d is greater than the maximum of %d.", p_device_id, XUSER_MAX_COUNT);
        return -1;
    }

    // Also note, we open the device in non-blocking mode so that if there are
    //  no events for the joystick, we don't end up sitting forever waiting for
    //  one to arrive.

    // Read-write is required for rumbling
    gamepad_states[p_device_id].file_desc = open(gamepad_states[p_device_id].dev_path,
                                                 O_RDWR | O_NONBLOCK);
    if(gamepad_states[p_device_id].file_desc != -1) {
        gamepad_states[p_device_id].connected = TRUE;
        gamepad_states[p_device_id].can_rumble = TRUE;
        return 0;
    } else if(errno == ENOENT) {
        // TODO: Should we write a warning in a debug sense?
        return -1;
    }

    syslog(LOG_WARNING, "Failed to open device %d (%s) for writing! Cannot vibrate. Reason: %s", p_device_id, gamepad_states[p_device_id].dev_path, strerror(errno));

    // Try to open just for reading (no rumbling capabilities)
    if(errno == EACCES) {
        gamepad_states[p_device_id].file_desc = open(gamepad_states[p_device_id].dev_path,
                                                     O_RDONLY | O_NONBLOCK);
        if(gamepad_states[p_device_id].file_desc != -1) {
            gamepad_states[p_device_id].connected = TRUE;
            return 0;
        }
    }

    syslog(LOG_ERR, "Failed to open device %d for either reading or writing! Reason: %s", p_device_id, strerror(errno));

    return -1;
}
```

It should be noted, that most of the code here is simply initializations after
calls to `open`, and warning/erroring code should `open` fail, all of which is
basically just standard C code. Of course, while simple, it is better to just
stick with libraries that are already cross-platform/open source, as they all
will do things like this internally already for you.

## Useful Tips

I should also mention fr a little bit some more useful tips and pitfalls that
are important to know about. The first is to re-iterate a point made earlier in
the document: be careful about compiler extensions when writing cross-platform
code, especially when porting code that was originally only compiled with MSVC.
A pretty egregious ones are that MSVC adds a constructor to the `std::exception`
class, allowing it to accept a string for the `what` parameter, which is not
standard C++ and will not compile in any of the Linux-based compilers. An easy
way around this is to simply use one of the more descriptive exception types,
such as `std::runtime_error`, which can all be found in the `<stdexcept>`
header. Another thing to watch out for is standard defined abiguities, such as
with `std::filesystem::path`. While `std::filesystem::path` is an amazing leap
forward, and you should _definitely_ use it for writing any sort of code that
needs to deal with files, it has a system-specific ambiguity in the standard,
mainly that the `.{,w,u8,u16,u32}string()`, `.c_str()`, `.native()`, and
`.operator string_type()` methods will return the path as the native type for
the current system. This means, that when compiling with MSVC's compiler, these
methods will return a wide character string, whereas on Linux they will return a
normal character string, which will cause problems if you attempt to assign them
to a variable. To fix this, there exist `std::generic_{,w,u8,u16,u32}string`
methods that will always return the expected types regardless of system.

Finally, one should also watch out for different standards of issuing warnings.
This problem mainly comes up when using MSVC, as it tends to allow more code
through than its Linux-based counterparts would. A good example of this is out
of order initializations in constructors, which is undefined behavior according
to the standard for good reason. MSVC does not warn for this by default on any
of the `/W{N}` flags, and it must be enabled explicitly with `/w"35038"` in the
compile flags. In general, switching to a Linux compiler will probably reveal
several warnings/potential problems that you didn't even know were occurring in
your code simply because MSVC wasn't warning for them.

## Final Thoughts

As should be hopefully clear, writing code to be cross-platform is very easy in
modern C++. So long as one is sure to follow proper C++ coding standards
deviating as little as possible from the official C++ standards document, uses
a modern version of one of the popular build system tools out there, and mainly
uses cross-platform or open source libraries, then making sure that your code
can and will run on nearly any operating system will be very easy.

