project(Androbot LANGUAGES CXX)

add_executable(Androbot
    main.cpp
    SerialPort.cpp
  )

  add_executable(MatrixTest MatrixTest.cpp)
  target_include_directories(MatrixTest PRIVATE ${CMAKE_SOURCE_DIR}/include)

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
        ${BINARY_DIR}/${CMAKE_BUILD_TYPE}/$<$<NOT:${WIN32}>:lib>realsense2$<$<AND:${WIN32},$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.$<IF:${WIN32},lib,a>
        ${BINARY_DIR}/libusb_install/lib/$<$<NOT:${WIN32}>:lib>usb.$<IF:$<BOOL:WIN32>,lib,a>
        ${BINARY_DIR}/third-party/realsense-file/${CMAKE_BUILD_TYPE}/$<$<NOT:${WIN32}>:lib>realsense-file$<$<AND:${WIN32},$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:d>.$<IF:${WIN32},lib,a>

        sfml-graphics
)

set_target_properties(Androbot
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED TRUE
        COMPILE_OPTIONS $<${WIN32}:/MTd>
)