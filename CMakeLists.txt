cmake_minimum_required(VERSION 3.1.0)

project(RealSenseDepthRenderer)

include(CMake/AddLibrealsense.cmake)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)