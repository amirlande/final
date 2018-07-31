cmake_minimum_required(VERSION 3.12)
project(final C)

set(CMAKE_C_STANDARD 90)

add_executable(final main.c gameUtils.c gameUtils.h cmake-build-debug/commands.c cmake-build-debug/commands.h)