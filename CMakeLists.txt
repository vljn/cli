cmake_minimum_required(VERSION 4.1)
project(cli)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Core/Interpreter.h
        src/Core/Interpreter.cpp
        src/Core/Parser/Parser.cpp
        src/Core/Parser/Parser.h
        src/Core/Command.cpp
        src/Core/Command.h
        src/Commands/Time.cpp
        src/Commands/Time.h
        src/Core/Parser/ParsedCommand.h
        src/Core/CommandFactory.cpp
        src/Core/CommandFactory.h
        src/Core/CommandInvocation.h
)
