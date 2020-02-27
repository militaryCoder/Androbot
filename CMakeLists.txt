cmake_minimum_required(VERSION 3.1.0)

project(RealSenseDepthRenderer)

set(TARGET_NAME ${CMAKE_PROJECT_NAME})

add_executable(${TARGET_NAME}
    src/cross_platform/main_cross_platform.cpp
)

if(WIN32)
    include(CMake/ConfigWin32.cmake)
elseif(UNIX AND NOT APPLE)
    include(CMake/ConfigUnix.cmake)
else()
    include(CMake/Config_macOS.cmake)
endif()

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${INCLUDE_DIRS}

    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
    ${LIBS}
)