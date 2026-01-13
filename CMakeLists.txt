cmake_minimum_required(VERSION 4.1)
project(cli)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Core/Interpreter.h
        src/Core/Interpreter.cpp
        src/Core/Parser/Parser.cpp
        src/Core/Parser/Parser.h
        src/Core/Command/Command.h
        src/Commands/Time.cpp
        src/Commands/Time.h
        src/Core/Parser/ParsedCommand.h
        src/Core/Command/CommandFactory.cpp
        src/Core/Command/CommandFactory.h
        src/Core/Command/CommandWithInputArgument.cpp
        src/Core/Command/CommandWithInputArgument.h
        src/Commands/Date.cpp
        src/Commands/Date.h
        src/Commands/Touch.cpp
        src/Commands/Touch.h
        src/Commands/Wc.cpp
        src/Commands/Wc.h
        src/Commands/Echo.cpp
        src/Commands/Echo.h
)
