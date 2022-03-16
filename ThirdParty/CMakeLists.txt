CMAKE_MINIMUM_REQUIRED(VERSION 3.19 FATAL_ERROR)

# Determine if this file has been included by ADD_SUBDIRECTORY or run directly from cmake
# If the file was included, it will then run a sub-cmake in order to configure and build
# the required dependencies during the main project's configure step.
IF(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

    SET(_source_dir ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(_binary_dir ${CMAKE_BINARY_DIR}/ThirdParty)

    # Set *_ROOT variables for config-based packages
    SET(glm_ROOT ${_binary_dir}/glm/cmake PARENT_SCOPE)
    SET(fmt_ROOT ${_binary_dir}/fmt PARENT_SCOPE)
    SET(SDL2_ROOT ${_binary_dir}/SDL2 PARENT_SCOPE)
    SET(nlohmann_json_ROOT ${_binary_dir}/nlohmann_json PARENT_SCOPE)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
        ${_binary_dir}/GTest
    )
    SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} PARENT_SCOPE)

    ###
    ### Configure
    ###

    FILE(MAKE_DIRECTORY ${_source_dir})

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            ${_source_dir}
        WORKING_DIRECTORY ${_binary_dir}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE(SEND_ERROR "${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    ###
    ### Build
    ###

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${_binary_dir}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE(SEND_ERROR "${_result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()

ELSE()

    LIST(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../CMake")
    
    INCLUDE(ExternalProject)

    PROJECT(ThirdParty)

    ###
    ### glm
    ###

    SET(glm_ROOT       ${CMAKE_BINARY_DIR}/glm/cmake)
    SET(glm_SOURCE_DIR ${CMAKE_BINARY_DIR}/glm)

    FIND_PACKAGE(glm CONFIG QUIET)

    IF(NOT glm_FOUND OR NOT TARGET glm::glm)
        ExternalProject_Add(
            ThirdParty_glm
            URL                 "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip"
            URL_HASH            MD5=69895110052f0d711c9c54fbf385f6f5
            SOURCE_DIR          ${glm_SOURCE_DIR}
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            UPDATE_COMMAND      ""
        )
    ENDIF()

    ###
    ### fmt
    ###

    SET(fmt_ROOT       ${CMAKE_BINARY_DIR}/fmt)
    SET(fmt_SOURCE_DIR ${CMAKE_BINARY_DIR}/fmt_src)
    SET(fmt_BINARY_DIR ${CMAKE_BINARY_DIR}/fmt_bin)

    FIND_PACKAGE(fmt 7.0.0 CONFIG QUIET)

    IF(NOT fmt_FOUND)
        ExternalProject_Add(
            ThirdParty_fmt
            URL                 "https://github.com/fmtlib/fmt/releases/download/8.0.1/fmt-8.0.1.zip"
            URL_HASH            MD5=21039fb4c826c81829a6c4d600e2b11c
            SOURCE_DIR          ${fmt_SOURCE_DIR}
            BINARY_DIR          ${fmt_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${fmt_ROOT}
                                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                -DFMT_DOC=OFF
                                -DFMT_TEST=OFF
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install
        )
    ENDIF()

    ###
    ### SDL2
    ###

    SET(SDL2_ROOT       ${CMAKE_BINARY_DIR}/SDL2)
    SET(SDL2_SOURCE_DIR ${CMAKE_BINARY_DIR}/SDL2_src)
    SET(SDL2_BINARY_DIR ${CMAKE_BINARY_DIR}/SDL2_bin)

    FIND_PACKAGE(SDL2 2.0.6 CONFIG QUIET)

    IF(CMAKE_GENERATOR STREQUAL "Visual Studio 16 2019")
        FIND_PACKAGE(patch REQUIRED)
        SET(_SDL2_PATCH_COMMAND ${patch_COMMAND} ${SDL2_SOURCE_DIR}/CMakeLists.txt ${CMAKE_SOURCE_DIR}/SDL2_vcruntime.patch )
    ENDIF()

    IF(NOT SDL2_FOUND)
        ExternalProject_Add(
            ThirdParty_SDL2
            URL                 "https://www.libsdl.org/release/SDL2-2.0.14.tar.gz"
            URL_HASH            MD5=76ed4e6da9c07bd168b2acd9bfefab1b
            SOURCE_DIR          ${SDL2_SOURCE_DIR}
            BINARY_DIR          ${SDL2_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                -DBUILD_SHARED_LIBS=ON
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install
            PATCH_COMMAND       ${_SDL2_PATCH_COMMAND}
        )
    ENDIF()

    ###
    ### nlohmann_json
    ###

    SET(nlohmann_json_ROOT       ${CMAKE_BINARY_DIR}/nlohmann_json)
    SET(nlohmann_json_SOURCE_DIR ${CMAKE_BINARY_DIR}/nlohmann_json_src)

    FIND_PACKAGE(nlohmann_json CONFIG QUIET)

    IF(NOT nlohmann_json_FOUND)
        ExternalProject_Add(
            ThirdParty_nlohmann_json
            URL                 "https://github.com/nlohmann/json/archive/refs/tags/v3.10.4.zip"
            URL_HASH            MD5=59c2a25e17b94d612fdb32a1a37378cf
            SOURCE_DIR          ${nlohmann_json_SOURCE_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${nlohmann_json_ROOT}
                                -DJSON_BuildTests=OFF
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
        )
    ENDIF()

    ###
    ### GTest
    ###

    SET(GTEST_ROOT       ${CMAKE_BINARY_DIR}/GTest)
    SET(GTEST_SOURCE_DIR ${CMAKE_BINARY_DIR}/GTest_src)
    SET(GTEST_BINARY_DIR ${CMAKE_BINARY_DIR}/GTest_bin)
    
    LIST(APPEND CMAKE_PREFIX_PATH
        ${GTEST_ROOT}
    )

    SET(GTEST_MSVC_SEARCH "MD")
    FIND_PACKAGE(GTest QUIET)

    IF(NOT GTEST_FOUND)
        ExternalProject_Add(
            ThirdParty_GTest
            URL                 "https://github.com/google/googletest/archive/release-1.11.0.tar.gz"
            URL_HASH            MD5=e8a8df240b6938bb6384155d4c37d937
            SOURCE_DIR          ${GTEST_SOURCE_DIR}
            BINARY_DIR          ${GTEST_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${GTEST_ROOT}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                                -DBUILD_SHARED_LIBS=ON
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config ${CMAKE_BUILD_TYPE}
        )
    ENDIF()

ENDIF()


