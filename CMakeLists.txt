cmake_minimum_required(VERSION 3.1.0)

project(RealSenseDepthRenderer)

include(CMake/AddLibrealsense.cmake)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
