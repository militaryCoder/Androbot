project(Androbot LANGUAGES CXX)

add_executable(Androbot main.cpp)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build Type is "${CMAKE_BUILD_TYPE})

target_include_directories(Androbot
    PRIVATE
        ${SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
)

set(IS_UNIX_OS 1)
if(WIN32)
    set(IS_UNIX_OS 0)
endif()

target_link_libraries(Androbot
    PRIVATE
        ${BINARY_DIR}/${CMAKE_BUILD_TYPE}/$<${IS_UNIX_OS}:lib>realsense2$<$<AND:$<NOT:${IS_UNIX_OS}>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.$<IF:$<BOOL:WIN32>,lib,a>
        ${BINARY_DIR}/libusb_install/lib/$<${IS_UNIX_OS}:lib>usb.$<IF:$<BOOL:WIN32>,lib,a>
        ${BINARY_DIR}/third-party/realsense-file/${CMAKE_BUILD_TYPE}/$<${IS_UNIX_OS}:lib>realsense-file$<$<AND:$<NOT:${IS_UNIX_OS}>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.$<IF:$<BOOL:WIN32>,lib,a>
)

target_compile_options(Androbot
    PRIVATE /MTd
)