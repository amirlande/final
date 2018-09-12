cmake_minimum_required(VERSION 3.12)
project(final C)

set(CMAKE_C_STANDARD 90)

add_executable(final main.c gameUtils.c gameUtils.h commands.c commands.h solver.h solver.c input_output.h input_output.c parser.c parser.h commandStruct.h commandStruct.c errorMessages.c errorMessages.h memoryAllocation.c memoryAllocation.h Stack.c Stack.h mainAux.c mainAux.h list.c list.h gameParams.c gameParams.h gurobi.c gurobi.h gurobi_c.h tests.c tests.h)

