cmake_minimum_required (VERSION 3.5)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_C_COMPILER "/usr/bin/clang")
set (CMAKE_C_FLAGS "-std=c11 -Os -Wall -Wextra -Wshadow")

file (GLOB SRCS
	client.c
)

project (client)
add_executable(client ${COMMON} ${SRCS})
