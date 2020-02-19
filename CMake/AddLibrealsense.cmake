include(ExternalProject)

if(NOT EXISTS $ENV{LIBREALSENSE2_SOURCE_DIR})
    set($ENV{LIBREALSENSE2_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/librealsense2)

    ExternalProject_Add(librealsense2
        PREFIX librealsense2
        GIT_REPOSITORY https://github.com/IntelRealSense/librealsense.git
        SOURCE_DIR $ENV{LIBREALSENSE2_SOURCE_DIR}
        BINARY_DIR ${CMAKE_BINARY_DIR}/librealsense
    )
endif()

# if(WIN32)
#     set(LIBREALSENSE2_LIBRARIES ${librealsense2_BINARY_DIR}/)
# endif()