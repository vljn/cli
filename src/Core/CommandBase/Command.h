#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include <iostream>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) = 0;
};

#endif //CLI_COMMAND_H