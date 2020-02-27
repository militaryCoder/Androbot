set(STANDARD_INCLUDE_PATH
    C:\\include
)

if(NOT EXISTS ${STANDARD_INCLUDE_PATH})
    message(FATAL_ERROR "No include directories found")
endif()

set(STANDARD_LIB_PATH
    C:\\libs
)

if(NOT EXISTS ${STANDARD_LIB_PATH})
    message(FATAL_ERROR "No libraries found")
endif()

set(LIBS
    ${STANDARD_LIB_PATH}\\librealsense\\usb.lib
    ${STANDARD_LIB_PATH}\\librealsense\\glfw3d.lib
    ${STANDARD_LIB_PATH}\\librealsense\\realsense2d.lib
    ${STANDARD_LIB_PATH}\\librealsense\\realsense2-gld.lib
    ${STANDARD_LIB_PATH}\\librealsense\\realsense-filed.lib
    ${STANDARD_LIB_PATH}\\librealsense\\realsense-viewer.lib
    ${STANDARD_LIB_PATH}\\librealsense\\rs-depth-quality.lib
    ${STANDARD_LIB_PATH}\\librealsense\\uvc_fwd.lib
)

message(STATUS "Win32 libraries: " ${LIBS})

set(INCLUDE_DIRS
    ${STANDARD_INCLUDE_PATH}\\librealsense
)

message(STATUS "Win32 include directories: " ${INCLUDE_DIRS})