
include(GetGitRevisionDescription)

# Get git hash
get_git_head_revision(GIT_REFSPEC PROJECT_VERSION_GIT_HASH)
string(SUBSTRING "${PROJECT_VERSION_GIT_HASH}" 0 8 PROJECT_VERSION_GIT_HASH)

# Parse version from git tags
git_describe(_version --tags)
if(_version)
    string(REGEX REPLACE 
        "^v([0-9]+)\\..*" "\\1"
        PROJECT_VERSION_MAJOR
        "${_version}"
    )
    string(REGEX REPLACE 
        "^v[0-9]+\\.([0-9]+).*" "\\1"
        PROJECT_VERSION_MINOR
        "${_version}"
    )
    string(REGEX REPLACE 
        "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1"
        PROJECT_VERSION_PATCH
        "${_version}"
    )
else()
    set(PROJECT_VERSION_MAJOR 0)
    set(PROJECT_VERSION_MINOR 0)
    set(PROJECT_VERSION_PATCH 0)
    
    message(WARNING "Failed to parse version from `git describe --tags`")
endif()

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
message(STATUS "${PROJECT_NAME} Version: ${PROJECT_VERSION}")
