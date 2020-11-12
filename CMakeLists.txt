cmake_minimum_required(VERSION 3.1.0)

project(Androbot LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(GLEW_INCLUDE_DIRS "")
set(GLEW_STATIC_LIB "")

if("${GLEW_INCLUDE_DIRS}" STREQUAL "" OR "${GLEW_STATIC_LIB}" STREQUAL "")
	message(WARNING "Variables needed by build system were not specified, please specify:\n"
		"GLEW_INCLUDE_DIRS\n"
		"GLEW_STATIC_LIB"
	)
endif()

add_subdirectory(src)