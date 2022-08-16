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

FIND_PACKAGE(glm CONFIG REQUIRED)

FIND_PACKAGE(fmt 7.0.0 CONFIG REQUIRED)

FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)

FIND_PACKAGE(cflags CONFIG REQUIRED)

FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)

FIND_PACKAGE(Qt6 COMPONENTS Widgets REQUIRED)

FIND_PACKAGE(Vulkan COMPONENTS glslc REQUIRED)

FIND_PACKAGE(Python3 COMPONENTS Interpreter REQUIRED)

SET(GTEST_MSVC_SEARCH "MD")
FIND_PACKAGE(GTest REQUIRED)

###
### Globals
###

INCLUDE(GetRuntimeDir)

GET_RUNTIME_DIR(SDL2::SDL2 _SDL2_runtime_dir)

GET_RUNTIME_DIR(GTest::gtest _GTest_runtime_dir)

SET(KIWI_RUNTIME_PATH_LIST
    ${_SDL2_runtime_dir}
    ${_GTest_runtime_dir}
    CACHE INTERNAL ""
)

SET(KIWI_SHADER_INCLUDE_PATH "" CACHE INTERNAL "")

###
### Targets
###

INCLUDE(ConfigureLibrary)
INCLUDE(AddLaunchTarget)

ADD_SUBDIRECTORY(Utility)

ADD_SUBDIRECTORY(Kiwi)

ADD_SUBDIRECTORY(Tools)

ADD_SUBDIRECTORY(Processors)

ADD_SUBDIRECTORY(Emulators)

###
### Launch Targets
###

ADD_LAUNCH_TARGET(
    "Kiwi"
    ${CMAKE_BINARY_DIR}
    $<TARGET_FILE:KiwiGUI>
)

FOREACH(_emulator_target ${KIWI_EMULATOR_TARGET_LIST})

    ADD_LAUNCH_TARGET(
        "Kiwi ${_emulator_target}"
        ${CMAKE_BINARY_DIR}
        $<TARGET_FILE:KiwiGUI>
        --emulator $<TARGET_FILE:${_emulator_target}>
    )

ENDFOREACH()

GENERATE_LAUNCH_TARGET_CONFIG()
