#ifndef CLI_COMMANDFACTORY_H
#define CLI_COMMANDFACTORY_H

#include <memory>
#include <unordered_map>

#include "../Parser/ParsedCommand.h"
#include "Command.h"

class CommandFactory {
public:
    static std::unique_ptr<Command> create(const ParsedCommand& pc);
private:
    using CommandPtr = std::unique_ptr<Command>;
    using ArgumentsVector = const std::vector<Token>&;
    using CommandCreator = CommandPtr (*)(ArgumentsVector);

    static std::unordered_map<std::string, CommandCreator> m_commandsMap;

    static CommandPtr createTouch(ArgumentsVector args);
    static CommandPtr createWc(ArgumentsVector args);
    static CommandPtr createEcho(ArgumentsVector args);
};

#endif //CLI_COMMANDFACTORY_H