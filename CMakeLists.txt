cmake_minimum_required (VERSION 3.5)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_C_COMPILER "/usr/bin/clang")
set (CMAKE_C_FLAGS "-std=gnu11 -Os -Wall -Wextra -Wshadow")

include_directories (
	utils
	utils/log
	utils/timer
	utils/cli
	node
	packet
)

file (GLOB SRCS
	utils/log/log.c
	utils/timer/timer.c
	utils/cli/cli.c
	node/node.c
	packet/packet.c
	main.c
)

project (ns)
add_executable(ns ${SRCS})
target_link_libraries (ns rt)
