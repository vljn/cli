#ifndef CLI_PROMPT_H
#define CLI_PROMPT_H

#include <utility>

#include "Core/CommandBase/Command.h"

class Prompt : public Command {
public:
    explicit Prompt(std::string promptString) : m_promptString(std::move(promptString)) { }

    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;

    bool consumesInput() override { return false; }
    bool producesOutput() override { return false; }
private:
    std::string m_promptString;
};

inline CommandResult Prompt::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    return {InterpreterAction::SetPromptString, nullptr, m_promptString};
}

#endif //CLI_PROMPT_H