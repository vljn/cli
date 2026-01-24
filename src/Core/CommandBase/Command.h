#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include <iostream>

enum class InterpreterAction {
    None,
    PushStream
};

class Command {
public:
    struct CommandResult {
        InterpreterAction action = InterpreterAction::None;
        std::istream* newStream = nullptr;
    };

    virtual ~Command() = default;

    virtual CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) = 0;
};

#endif //CLI_COMMAND_H