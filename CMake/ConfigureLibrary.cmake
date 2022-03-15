
INCLUDE(ConfigureFileList)

MACRO(CONFIGURE_LIBRARY _target)

    ###
    ### Source Files
    ###

    FILE(
        GLOB_RECURSE
        _source_list
        "Public/*.h"
        "Public/*.hpp"
        "Private/*.h"
        "Private/*.hpp"
        "Private/*.c"
        "Private/*.cpp"
    )

    FILE(
        GLOB_RECURSE
        _source_input_list
        "Public/*.in"
        "Private/*.in"
    )

    CONFIGURE_FILE_LIST("${_source_input_list}" _source_output_list)

    ###
    ### Target Configuration
    ###

    TARGET_SOURCES(
        ${_target}
        PRIVATE
            ${_source_list}
            ${_source_output_list}
    )

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/Public
            ${CMAKE_CURRENT_BINARY_DIR}/Public
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Private
            ${CMAKE_CURRENT_BINARY_DIR}/Private
    )

    TARGET_COMPILE_DEFINITIONS(
        ${_target}
        PUBLIC
            # Disable VS "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>

            $<$<BOOL:${KIWI_BENCHMARK}>:KIWI_ENABLE_BENCHMARK>
    )

    TARGET_COMPILE_OPTIONS(
        ${_target}
        PUBLIC
            # Configure exception handling model
            $<$<CXX_COMPILER_ID:MSVC>: /EHs>
    )

    ###
    ### Tests
    ###

    IF(KIWI_TEST)
        FILE(
            GLOB
            _test_source_list
            "Tests/*.cpp"
        )

        FOREACH(_test_source ${_test_source_list})
            CMAKE_PATH(GET _test_source STEM _stem)
            SET(_test_target "${_target}_${_stem}")

            ADD_EXECUTABLE(
                ${_test_target}
                ${_test_source}
            )

            TARGET_LINK_LIBRARIES(
                ${_test_target}
                PRIVATE
                    ${_target}
                    GTest::gtest
            )

            ADD_TEST(
                NAME ${_test_target}
                COMMAND $<TARGET_FILE:${_test_target}>
                WORKING_DIRECTORY Tests
            )
            
            IF(WIN32)
                STRING(REPLACE ";" "\\\\;" _env "PATH=${KIWI_RUNTIME_PATH};$ENV{PATH}")
            ELSE()
                STRING(REPLACE ";" ":" _env "LD_LIBRARY_PATH=${KIWI_RUNTIME_PATH}")
            ENDIF()

            SET_TESTS_PROPERTIES(
                ${_test_target}
                PROPERTIES
                    ENVIRONMENT ${_env}
            )
        ENDFOREACH()
    ENDIF()

    LIST(APPEND KIWI_EMULATOR_TARGET_LIST ${_target})
    SET(KIWI_EMULATOR_TARGET_LIST ${KIWI_EMULATOR_TARGET_LIST} PARENT_SCOPE)

ENDMACRO()
