cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(ExternalProject)

ExternalProject_Add(
    librealsense2
    PREFIX ${CMAKE_SOURCE_DIR}/3rd-party/librealsense

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

add_subdirectory(src)