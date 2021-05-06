cmake_minimum_required(VERSION 3.15.0)

find_package(PortAudio REQUIRED)

add_library(AudioEngine STATIC
    Headers/AE_AudioMath.hpp            AE_AudioMath.cpp
    Headers/AE_Events.hpp
    Headers/AE_RingBuffer.hpp           AE_RingBuffer.cpp
    Headers/AudioAssetManager.h         AudioAssetManager.cpp
    Headers/AudioEngine.hpp             AudioEngine.cpp
    Headers/AudioStorage.hpp            AudioStorage.cpp
    Headers/BusInterface.h
    Headers/Compressor.h                Compressor.cpp
    Headers/DisplayPrimer.h
    Headers/DoubleBuffer.h              DoubleBuffer.cpp
    Headers/DSPBase.h
    Headers/DSPFactory.h                DSPFactory.cpp
    Headers/Envelopes.h                 Envelopes.cpp
    Headers/EventHandler.hpp            EventHandler.cpp
    Headers/FileDefinitions.h
    Headers/Filter.h                    Filter.cpp
    Headers/FilterFactory.h             FilterFactory.cpp
    Headers/IOTarget.h                  IOTarget.cpp
    Headers/MixerObjects.h
    Headers/Parameters.h                Parameters.cpp
    Headers/ProcessingSettings.h        ProcessingSettings.cpp
    Headers/Serializer.h                Serializer.cpp
    Headers/Voice.h                     Voice.cpp
    )

target_include_directories(AudioEngine
    PUBLIC SYSTEM Headers
    PRIVATE PortAudio)

target_link_libraries(AudioEngine
    PRIVATE PortAudio)

# build up resources list
list(APPEND resources ${PortAudio_RESOURCES})
set(AudioEngine_RESOURCES ${resources} CACHE STRING "AudioEngine_RESOURCES" FORCE)

set_target_properties(AudioEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

if(MSVC)
    target_compile_options(AudioEngine PRIVATE /W0)
else()
    target_compile_options(AudioEngine PRIVATE -w)
endif()

