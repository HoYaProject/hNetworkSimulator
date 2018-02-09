cmake_minimum_required (VERSION 3.5)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_C_COMPILER "/usr/bin/clang")
set (CMAKE_C_FLAGS "-std=gnu11 -Os -Wall -Wextra -Wshadow")

include_directories (
	cli
	server
	utils
)

file (GLOB SERVER_SRCS
	server/server.c
)

file (GLOB CLIENT_SRCS
	client/client.c
)

file (GLOB SRCS
	cli/cli.c
	main.c
)

project (ns)
add_executable(ns ${SRCS})

project (server)
add_executable(server ${SERVER_SRCS})

project (client)
add_executable(client ${CLIENT_SRCS})
