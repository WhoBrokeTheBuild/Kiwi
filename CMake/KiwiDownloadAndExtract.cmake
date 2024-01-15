# DownloadAndExtract.cmake
#
# This will download the archive from the specified ${_url}, and extract it
# into the folder ${_destination}. If the top-level content of the archive 
# is a single folder, that will be moved to ${_destination} instead.
#

function(kiwi_download_and_extract _url _destination)
    if(NOT EXISTS ${_destination})
        cmake_path(GET _url FILENAME _filename)
        cmake_path(GET _url STEM _stem)

        set(_archive ${CMAKE_CURRENT_BINARY_DIR}/${_filename})
        set(_staging ${CMAKE_CURRENT_BINARY_DIR}/${_stem})

        file(DOWNLOAD
            ${_url}
            ${_archive}
            STATUS _status
        )

        # FILE(DOWNLOAD) will fail silently
        list(GET _status 0 _status_code)
        list(GET _status 1 _status_message)
        if(NOT _status_code EQUAL 0)
            message(FATAL_ERROR "Failed to download ${_url} ${_status_message}")
        endif()

        file(ARCHIVE_EXTRACT
            INPUT ${_archive}
            DESTINATION ${_staging}
        )

        file(GLOB _file_list ${_staging}/*)
        list(LENGTH _file_list _file_count)

        if(_file_count EQUAL 1 AND IS_DIRECTORY ${_file_list})
            file(RENAME ${_file_list} ${_destination})
            file(REMOVE ${_staging})
        else()
            file(RENAME ${_staging} ${_destination})
        endif()

        file(REMOVE ${_archive})
    endif()
endfunction()
