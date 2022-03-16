CMAKE_MINIMUM_REQUIRED(VERSION 3.19 FATAL_ERROR)

# Allow for custom CMake modules
LIST(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

# Allow PROJECT() to manage versions
IF(POLICY CMP0048)
    CMAKE_POLICY(SET CMP0048 NEW)
ENDIF()

# Process DEPFILEs sent to Ninja
IF(POLICY CMP0116)
    CMAKE_POLICY(SET CMP0116 NEW)
ENDIF()

PROJECT(Kiwi)

INCLUDE(ProjectVersion)

###
### Options
###

OPTION(KIWI_BENCHMARK
    "Build Kiwi with benchmarking"
    OFF
)


OPTION(KIWI_TEST
    "Build Kiwi test suite"
    OFF
)

###
### Global Configuration
###

IF(KIWI_TEST)
    ENABLE_TESTING()
ENDIF()

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

###
### Third Party Dependencies
###

ADD_SUBDIRECTORY(ThirdParty)

FIND_PACKAGE(fmt 7.0.0 CONFIG REQUIRED)

FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)

FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)

FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

FIND_PACKAGE(Python3 COMPONENTS Interpreter REQUIRED)

SET(GTEST_MSVC_SEARCH "MD")
FIND_PACKAGE(GTest REQUIRED)

INCLUDE(GetRuntimeDir)

GET_RUNTIME_DIR(SDL2::SDL2 _SDL2_runtime_dir)
GET_RUNTIME_DIR(GTest::gtest _GTest_runtime_dir)

LIST(APPEND KIWI_RUNTIME_PATH
    ${_SDL2_runtime_dir}
    ${_GTest_runtime_dir}
)

###
### Assets
###

ADD_SUBDIRECTORY(ROMs)

SET(KIWI_ROMS_DIRECTORY ${CMAKE_SOURCE_DIR}/ROMs)

###
### Targets
###

INCLUDE(ConfigureLibrary)

ADD_SUBDIRECTORY(Kiwi)

ADD_SUBDIRECTORY(Processors)

SET(KIWI_EMULATOR_TARGET_LIST "")
ADD_SUBDIRECTORY(Emulators)

ADD_SUBDIRECTORY(Tools)

###
### Launch Targets
###

INCLUDE(AddLaunchTarget)

ADD_LAUNCH_TARGET("Kiwi" KiwiGUI)

FOREACH(_target ${KIWI_EMULATOR_TARGET_LIST})

    ADD_LAUNCH_TARGET(
        ${_target}
        KiwiGUI
        --emulator ${_target}
    )

ENDFOREACH()