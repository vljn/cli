#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H

#include <iostream>
#include <memory>

enum class InterpreterAction {
    None,
    PushStream,
    SetPromptString
};

struct CommandResult {
    InterpreterAction action = InterpreterAction::None;
    std::unique_ptr<std::istream> newStream;
    const std::string newPromptString;
};

class Command {
public:
    virtual ~Command() = default;

    virtual CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) = 0;
};

#endif //CLI_COMMAND_H