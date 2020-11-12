project(AndrobotApp)

add_executable(Androbot
    main.cpp
    SerialPort.cpp
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build Type is " ${CMAKE_BUILD_TYPE})

include(${CMAKE_SOURCE_DIR}/CMake/AddLibRealSense.cmake)

target_include_directories(Androbot
    PRIVATE
    ${LIBREALSENSE_SRC_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Androbot
    ${LIBREALSENSE_BIN_DIR}/${CMAKE_BUILD_TYPE}/$<$<NOT:${WIN32}>:lib>realsense2$<$<AND:${WIN32},$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.$<IF:${WIN32},lib,a>
    ${LIBREALSENSE_BIN_DIR}/libusb_install/lib/$<$<NOT:${WIN32}>:lib>usb.$<IF:$<BOOL:WIN32>,lib,a>
    ${LIBREALSENSE_BIN_DIR}/third-party/realsense-file/${CMAKE_BUILD_TYPE}/$<$<NOT:${WIN32}>:lib>realsense-file$<$<AND:${WIN32},$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.$<IF:${WIN32},lib,a>
)

target_include_directories(Androbot
    PRIVATE
    ${GLEW_SRC_DIR}/include
)

#target_link_libraries(Androbot
#    ${GLEW_BIN_DIR}/lib/${CMAKE_BUILD_TYPE}/libglew32$<$<AND:${WIN32},$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.lib
#)

set_target_properties(Androbot
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED TRUE
        COMPILE_OPTIONS $<${WIN32}:/MTd>
)