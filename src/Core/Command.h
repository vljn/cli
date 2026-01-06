#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include <iostream>
#include "CommandInvocation.h"

class CommandInvocation;

class Command {
public:
    Command(CommandInvocation invocation);
    virtual ~Command() = default;

    virtual void execute(std::istream* in, std::ostream* out) = 0;
protected:
    std::vector<Argument> args;
};

#endif //CLI_COMMAND_H