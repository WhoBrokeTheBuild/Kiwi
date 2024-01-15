cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# Allow for custom CMake modules
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

project(Kiwi)

include(KiwiProjectVersion)

###
### Options
###

option(KIWI_BENCHMARK
    "Build Kiwi with benchmarking"
    OFF
)


option(KIWI_TEST
    "Build Kiwi test suite"
    OFF
)

###
### Global Configuration
###

if(KIWI_TEST)
    enable_testing()
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
    )
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

###
### Third Party Dependencies
###

# TODO: Improve
set(_saved_log_level ${CMAKE_MESSAGE_LOG_LEVEL})
set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

include(FetchContent)

FetchContent_Declare(
    cflags
    GIT_REPOSITORY "https://github.com/WhoBrokeTheBuild/cflags.git"
    GIT_TAG        "v3.0.3"
    FIND_PACKAGE_ARGS
)

set(SDL_STATIC OFF)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG        "release-2.28.5"
    FIND_PACKAGE_ARGS
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG        "0.9.9.8"
    FIND_PACKAGE_ARGS
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
    GIT_TAG        "10.2.1"
    FIND_PACKAGE_ARGS
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY "https://github.com/nlohmann/json.git"
    GIT_TAG        "v3.11.3"
    FIND_PACKAGE_ARGS
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG        "v1.14.0"
    FIND_PACKAGE_ARGS NAMES GTest
)

FetchContent_MakeAvailable(cflags SDL2 glm fmt nlohmann_json googletest)

set(CMAKE_MESSAGE_LOG_LEVEL ${_saved_log_level})


find_package(Qt6 COMPONENTS Widgets REQUIRED)

find_package(Vulkan COMPONENTS glslc REQUIRED)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

###
### Globals
###

include(GetRuntimeDir)

get_runtime_dir(SDL2::SDL2 _SDL2_runtime_dir)

get_runtime_dir(GTest::gtest _GTest_runtime_dir)

get_runtime_dir(Qt6::Widgets _Qt_runtime_dir)

set(KIWI_RUNTIME_PATH_LIST
    ${_SDL2_runtime_dir}
    ${_GTest_runtime_dir}
    ${_Qt_runtime_dir}
    CACHE INTERNAL ""
)

set(KIWI_SHADER_INCLUDE_PATH "" CACHE INTERNAL "")

message(STATUS ${KIWI_RUNTIME_PATH_LIST})

###
### Targets
###

include(KiwiConfigureLibrary)
include(KiwiAddLaunchTarget)

add_subdirectory(Utility)

add_subdirectory(Kiwi)

add_subdirectory(Tools)

add_subdirectory(Processors)

add_subdirectory(Emulators)

###
### Launch Targets
###

kiwi_add_launch_target(
    "Kiwi"
    ${CMAKE_CURRENT_BINARY_DIR}
    $<TARGET_FILE:KiwiGUI>
)

foreach(_emulator_target ${KIWI_EMULATOR_TARGET_LIST})

    kiwi_add_launch_target(
        "Kiwi ${_emulator_target}"
        ${CMAKE_CURRENT_BINARY_DIR}
        $<TARGET_FILE:KiwiGUI>
        --emulator $<TARGET_FILE:${_emulator_target}>
    )

endforeach()

kiwi_generate_launch_target_config()
