cmake_minimum_required (VERSION 3.5)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_C_COMPILER "/usr/bin/clang")
set (CMAKE_C_FLAGS "-std=gnu11 -Os -Wall -Wextra -Wshadow")

include_directories (
	utils
	utils/log
	utils/cli
)

file (GLOB SRCS
	utils/log/log.c
	utils/cli/cli.c
	main.c
)

project (ns)
add_executable(ns ${SRCS})
