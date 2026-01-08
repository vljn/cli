#ifndef CLI_COMMANDWITHARGUMENTS_H
#define CLI_COMMANDWITHARGUMENTS_H
#include <memory>
#include "Command.h"
#include "CommandInvocation.h"

class CommandWithArguments : public Command {
public:
    CommandWithArguments(std::unique_ptr<CommandInvocation> invocation);
    ~CommandWithArguments() override = default;

    void execute(std::istream *in, std::ostream *out) override = 0;
protected:
    std::unique_ptr<CommandInvocation> m_invocation;
};


#endif //CLI_COMMANDWITHARGUMENTS_H