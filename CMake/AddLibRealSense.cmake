include(ExternalProject)
include(${CMAKE_SOURCE_DIR}/CMake/ThirdPartyDefs.cmake)

ExternalProject_Add(
    librealsense2
    SOURCE_DIR ${THIRD_PARTY_ROOT}/librealsense
    BINARY_DIR ${THIRD_PARTY_BIN_DIR}/librealsense
    TMP_DIR ${THIRD_PARTY_TMP_DIR}/librealsense
    STAMP_DIR ${THIRD_PARTY_STAMP_DIR}/librealsense
    LOG_DIR ${THIRD_PARTY_LOG_DIR}/librealsense

    GIT_REPOSITORY https://github.com/IntelRealSense/librealsense
    GIT_TAG v2.38.1
    GIT_SHALLOW ON
    GIT_PROGRESS ON

    CMAKE_ARGS
        -D BUILD_SHARED_LIBS=OFF
        -D BUILD_EXAMPLES=OFF
        -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(librealsense2 BINARY_DIR)
ExternalProject_Get_Property(librealsense2 SOURCE_DIR)

set(LIBREALSENSE_SRC_DIR ${SOURCE_DIR})
set(LIBREALSENSE_BIN_DIR ${BINARY_DIR})