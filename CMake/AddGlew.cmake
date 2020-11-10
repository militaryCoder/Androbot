include(ExternalProject)
include(${CMAKE_SOURCE_DIR}/CMake/ThirdPartyDefs.cmake)

ExternalProject_Add(
	glew
    SOURCE_DIR ${THIRD_PARTY_ROOT}/glew
	BINARY_DIR ${THIRD_PARTY_BIN_DIR}/glew
    TMP_DIR ${THIRD_PARTY_TMP_DIR}/glew
    STAMP_DIR ${THIRD_PARTY_STAMP_DIR}/glew
    LOG_DIR ${THIRD_PARTY_LOG_DIR}/glew

    GIT_REPOSITORY https://github.com/nigels-com/glew
    GIT_TAG glew-2.1.0
    GIT_SHALLOW ON
    GIT_PROGRESS ON

    CMAKE_ARGS
        -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

    CONFIGURE_COMMAND "cmake ${THIRD_PARTY_ROOT}/glew/build/cmake"
)

ExternalProject_Get_Property(glew SOURCE_DIR)
ExternalProject_Get_Property(glew BINARY_DIR)

set(GLEW_SRC_DIR ${SOURCE_DIR})
set(GLEW_BIN_DIR ${BINARY_DIR})