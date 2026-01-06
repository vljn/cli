#ifndef CLI_COMMANDFACTORY_H
#define CLI_COMMANDFACTORY_H

#include <memory>
#include "Parser/ParsedCommand.h"
#include "Command.h"

class CommandFactory {
public:
    static std::unique_ptr<Command> create(ParsedCommand& pc);
};


#endif //CLI_COMMANDFACTORY_H