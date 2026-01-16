#include "Command.h"
#include "CommandFactory.h"
#include "../../Commands/Date.h"
#include "../../Commands/Echo.h"
#include "../../Commands/Time.h"
#include "../../Commands/Touch.h"
#include "../../Commands/Wc.h"

std::unordered_map<std::string, CommandFactory::CommandCreator> CommandFactory::m_commandsMap = {
    {
        "time", [](const auto&) -> std::unique_ptr<Command> { return std::make_unique<Time>(); }
    },
    {
        "date", [](const auto&) -> std::unique_ptr<Command> { return std::make_unique<Date>(); }
    },
    {
       "touch", createTouch
    },
    {
        "wc", createWc
    },
    {
        "echo", createEcho
    }
};

CommandFactory::CommandPtr CommandFactory::createTouch(ArgumentsVector args) {
    if (args.size() > 1)
        throw std::runtime_error("Number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("Argument not given");
    if (args[0].type != TokenType::Filename)
        throw std::runtime_error("Filename argument expected without quotes");
    return std::make_unique<Touch>(args[0].value);
}

CommandFactory::CommandPtr CommandFactory::createWc(ArgumentsVector args) {
    if (args.size() > 2)
        throw std::runtime_error("Number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("Argument and option not given");
    if (args.size() == 1) {
        if (args[0].type != TokenType::Option)
            throw std::runtime_error("Option not given");
        if (args[0].value != "w" and args[0].value != "c")
            throw std::runtime_error("Invalid option: expected '-w' or '-c'");
        auto mode = args[0].value == "w" ? Wc::Mode::Words : Wc::Mode::Chars;
        return std::make_unique<Wc>(std::nullopt, mode);
    }
    if (args[0].type != TokenType::Option || args[1].type == TokenType::Option) {
        throw std::runtime_error("Expected an option followed by an argument");
    }
    if (args[0].value != "w" and args[0].value != "c")
        throw std::runtime_error("Invalid option: expected '-w' or '-c'");
    Argument argument{args[1].value, args[1].type == TokenType::QuotedString};
    auto mode = args[0].value == "w" ? Wc::Mode::Words : Wc::Mode::Chars;
    return std::make_unique<Wc>(argument, mode);
}

CommandFactory::CommandPtr CommandFactory::createEcho(ArgumentsVector args) {
    if (args.size() > 1)
        throw std::runtime_error("Number of arguments greater than expected");
    if (args.size() == 1) {
        if (args[0].type == TokenType::Option)
            throw std::runtime_error("Argument not given");
        Argument argument{args[0].value, args[0].type == TokenType::QuotedString};
        return std::make_unique<Echo>(argument);
    }
    return std::make_unique<Echo>(std::nullopt);
}

std::unique_ptr<Command> CommandFactory::create(const ParsedCommand& pc) {
    const auto& name = pc.name;
    const auto& tokens = pc.tokens;
    auto it = m_commandsMap.find(name);
    if (it == m_commandsMap.end())
        throw std::runtime_error("Unknown command: " + name);
    return it->second(tokens);
}
