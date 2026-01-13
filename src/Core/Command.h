#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include <iostream>

class Command {
public:
    virtual ~Command() = default;

    virtual bool consumesInput() = 0;
    virtual bool producesOutput() = 0;

    virtual void execute(std::istream& in, std::ostream& out) = 0;
};

#endif //CLI_COMMAND_H