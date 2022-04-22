
MACRO(DEFINE_EMULATOR _target)
    
    ADD_LIBRARY(${_target} SHARED "")

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            OUTPUT_NAME "Kiwi${_target}"
    )
    
    CONFIGURE_LIBRARY(${_target})

    TARGET_LINK_LIBRARIES(
        ${_target}
        PUBLIC
            Kiwi
    )

    SET(KIWI_EMULATOR_TARGET_LIST
        ${KIWI_EMULATOR_TARGET_LIST}
        ${_target}
        CACHE INTERNAL ""
    )

    SET(KIWI_RUNTIME_PATH_LIST
        ${KIWI_RUNTIME_PATH_LIST}
        ${CMAKE_CURRENT_BINARY_DIR}
        CACHE INTERNAL ""
    )

ENDMACRO()

###
### Emulator Libraries & Targets
###

UNSET(KIWI_EMULATOR_TARGET_LIST CACHE)

ADD_SUBDIRECTORY(CHIP-8)
ADD_SUBDIRECTORY(GameBoy)
ADD_SUBDIRECTORY(NES)
