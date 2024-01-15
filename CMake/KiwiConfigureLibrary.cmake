# ConfigureLibrary.cmake
#
# This will fully configure a library target with the following things:
# * Add source files from:
#   * Public/*.h
#   * Public/*.hpp
#   * Private/*.h
#   * Private/*.hpp
#   * Private/*.c
#   * Private/*.cpp
# * Configure and add source files from:
#   * Public/*.in
#   * Private/*.in
# * Configure include directories
# * Configure standard compile definitions and options
# * Build and add tests from Tests/*.cpp
#

macro(kiwi_configure_library _target)

    ###
    ### Source Files
    ###

    file(
        GLOB_RECURSE
        _source_list
        "Public/*.h"
        "Public/*.hpp"
        "Private/*.h"
        "Private/*.hpp"
        "Private/*.c"
        "Private/*.cpp"
    )

    file(
        GLOB_RECURSE
        _source_input_list
        "Public/*.in"
        "Private/*.in"
    )

    foreach(_input IN LISTS _source_input_list)
        # Replace leading source directory in path with binary directory
        string(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            _output
            ${_input}
        )

        # Remove the ".in"
        string(REGEX
            MATCH "^(.*)\\.[^.]*$"
            _output
            ${_output}
        )
        set(_output ${CMAKE_MATCH_1})

        configure_file(${_input} ${_output})

        list(APPEND _source_output_list ${_output})
    endforeach()

    ###
    ### Target Configuration
    ###

    target_sources(
        ${_target}
        PRIVATE
            ${_source_list}
            ${_source_output_list}
    )

    target_include_directories(
        ${_target}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/Public
            ${CMAKE_CURRENT_BINARY_DIR}/Public
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Private
            ${CMAKE_CURRENT_BINARY_DIR}/Private
    )

    target_compile_definitions(
        ${_target}
        PUBLIC
            # Disable VS "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>

            $<$<BOOL:${KIWI_BENCHMARK}>:KIWI_ENABLE_BENCHMARK>
    )

    target_compile_options(
        ${_target}
        PUBLIC
            # Configure exception handling model
            $<$<CXX_COMPILER_ID:MSVC>: /EHs>

            # Enable most warnings, disable unknown pragmas warning
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas -Wno-nullability-completeness -Wno-self-assign-overloaded>
            $<$<CXX_COMPILER_ID:MSVC>:  /wd4068>
    )

    ###
    ### Tests
    ###

    if(KIWI_TEST)
        file(
            GLOB
            _test_source_list
            "Tests/*.cpp"
        )

        foreach(_test_source ${_test_source_list})
            cmake_path(GET _test_source STEM _stem)
            set(_test_target "${_target}_${_stem}")

            add_executable(
                ${_test_target}
                ${_test_source}
            )

            target_link_libraries(
                ${_test_target}
                PRIVATE
                    ${_target}
                    GTest::gtest
            )

            add_test(
                NAME ${_test_target}
                COMMAND $<TARGET_FILE:${_test_target}>
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Tests
            )
            
            if(WIN32)
                string(REPLACE ";" "\\\\;" _env "PATH=${KIWI_RUNTIME_PATH_LIST};$ENV{PATH}")
            else()
                string(REPLACE ";" ":" _env "LD_LIBRARY_PATH=${KIWI_RUNTIME_PATH_LIST}")
            endif()

            set_tests_properties(
                ${_test_target}
                PROPERTIES
                    ENVIRONMENT ${_env}
            )

            kiwi_add_launch_target(
                "Kiwi Test ${_test_target}"
                ${CMAKE_CURRENT_SOURCE_DIR}/Tests
                $<TARGET_FILE:${_test_target}>
                "--verbose"
            )

        endforeach()
    endif()
endmacro()
