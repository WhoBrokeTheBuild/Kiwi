# DownloadAndExtract.cmake
#
# This will download the archive from the specified ${_url}, and extract it
# into the folder ${_destination}. If the top-level content of the archive 
# is a single folder, that will be moved to ${_destination} instead.
#

FUNCTION(DOWNLOAD_AND_EXTRACT _url _destination)
    IF(NOT EXISTS ${_destination})
        CMAKE_PATH(GET _url FILENAME _filename)
        CMAKE_PATH(GET _url STEM _stem)

        SET(_archive ${CMAKE_CURRENT_BINARY_DIR}/${_filename})
        SET(_staging ${CMAKE_CURRENT_BINARY_DIR}/${_stem})

        FILE(DOWNLOAD
            ${_url}
            ${_archive}
            STATUS _status
        )

        # FILE(DOWNLOAD) will fail silently
        LIST(GET _status 0 _status_code)
        LIST(GET _status 1 _status_message)
        IF(NOT _status_code EQUAL 0)
            MESSAGE(FATAL_ERROR "Failed to download ${_url} ${_status_message}")
        ENDIF()

        FILE(ARCHIVE_EXTRACT
            INPUT ${_archive}
            DESTINATION ${_staging}
        )

        FILE(GLOB _file_list ${_staging}/*)
        LIST(LENGTH _file_list _file_count)

        IF(_file_count EQUAL 1 AND IS_DIRECTORY ${_file_list})
            FILE(RENAME ${_file_list} ${_destination})
            FILE(REMOVE ${_staging})
        ELSE()
            FILE(RENAME ${_staging} ${_destination})
        ENDIF()

        FILE(REMOVE ${_archive})
    ENDIF()
ENDFUNCTION()
