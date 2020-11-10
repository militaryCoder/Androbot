cmake_minimum_required(VERSION 3.1.0)

project(Androbot LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

if($ENV{GLEW_STATIC_LIB_PATH} NOT EXISTS)
    message(FATAL_ERROR "No path to glew library specified. Specify it via environment variable like typing in the terminal:\n"
    "$ export GLEW_STATIC_LIB_PATH=<path_to_glew_lib>")
endif()

add_subdirectory(src)