
INCLUDE(GetGitRevisionDescription)

# Get git hash
GET_GIT_HEAD_REVISION(GIT_REFSPEC PROJECT_VERSION_GIT_HASH)
STRING(SUBSTRING "${PROJECT_VERSION_GIT_HASH}" 0 8 PROJECT_VERSION_GIT_HASH)

# Parse version from git tags
GIT_DESCRIBE(_version --tags)
IF(_version)
    STRING(REGEX REPLACE 
        "^v([0-9]+)\\..*" "\\1"
        PROJECT_VERSION_MAJOR
        "${_version}"
    )
    STRING(REGEX REPLACE 
        "^v[0-9]+\\.([0-9]+).*" "\\1"
        PROJECT_VERSION_MINOR
        "${_version}"
    )
    STRING(REGEX REPLACE 
        "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1"
        PROJECT_VERSION_PATCH
        "${_version}"
    )
ELSE()
    SET(PROJECT_VERSION_MAJOR 0)
    SET(PROJECT_VERSION_MINOR 0)
    SET(PROJECT_VERSION_PATCH 0)
    
    MESSAGE(WARNING "Failed to parse version from `git describe --tags`")
ENDIF()

SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
MESSAGE(STATUS "${PROJECT_NAME} Version: ${PROJECT_VERSION}")
