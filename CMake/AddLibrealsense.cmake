include(ExternalProject)

set($ENV{LIBREALSENSE2_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/3rd-party/librealsense2)
set(LIBREALSENSE_BUILD_COMMAND "mkdir "${CMAKE_BINARY_DIR}"/librealsense; cd librealsense; cmake "$ENV{LIBREALSENSE2_SOURCE_DIR}; "cmake --build .")

if(NOT EXISTS $ENV{LIBREALSENSE2_SOURCE_DIR})
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/librealsense)
        set(LIBREALSENSE_SHOULD_BUILD TRUE)

    ExternalProject_Add(librealsense2
        PREFIX librealsense2
        GIT_REPOSITORY https://github.com/IntelRealSense/librealsense.git
        SOURCE_DIR $ENV{LIBREALSENSE2_SOURCE_DIR}
        BINARY_DIR ${CMAKE_BINARY_DIR}/librealsense

        UPDATE_COMMAND ${GIT_EXECUTABLE} pull origin master

        BUILD_COMMAND $<${LIBREALSENSE_SHOULD_BUILD}:${LIBREALSENSE_BUILD_COMMAND}>
        BUILD_ALWAYS OFF
        BUILD_IN_SOURCE OFF
    )
endif()

# if(WIN32)
#     set(LIBREALSENSE2_LIBRARIES ${librealsense2_BINARY_DIR}/)
# endif()