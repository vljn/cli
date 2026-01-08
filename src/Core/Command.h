#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include <iostream>

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    virtual void execute(std::istream* in, std::ostream* out) = 0;
};

#endif //CLI_COMMAND_H