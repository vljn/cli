#include <unordered_set>

#include "Command.h"
#include "CommandFactory.h"
#include "../../Commands/Batch.h"
#include "../../Commands/Date.h"
#include "../../Commands/Echo.h"
#include "../../Commands/Time.h"
#include "../../Commands/Touch.h"
#include "../../Commands/Wc.h"
#include "../../Commands/Prompt.h"
#include "../../Exceptions/InvalidOptionsException.h"

std::unordered_map<std::string, CommandFactory::CommandCreator> CommandFactory::m_commandsMap = {
    {
        "time", createTime
    },
    {
        "date", createDate
    },
    {
       "touch", createTouch
    },
    {
        "wc", createWc
    },
    {
        "echo", createEcho
    },
    {
        "batch", createBatch
    },
    {
        "prompt", createPrompt
    }
};

CommandFactory::CommandPtr CommandFactory::createTime(ArgumentsVector args) {
    validateOptions("time", args, std::unordered_set<std::string>{});
    if (!args.empty()) {
        throw std::runtime_error("number of arguments greater than expected");
    }
    return std::make_unique<Time>();
}

CommandFactory::CommandPtr CommandFactory::createDate(ArgumentsVector args) {
    validateOptions("date", args, std::unordered_set<std::string>{});
    if (!args.empty()) {
        throw std::runtime_error("number of arguments greater than expected");
    }
    return std::make_unique<Date>();
}

CommandFactory::CommandPtr CommandFactory::createTouch(ArgumentsVector args) {
    validateOptions("touch", args, std::unordered_set<std::string>{});
    if (args.size() > 1)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("argument not given");
    if (args[0].type != TokenType::Filename)
        throw std::runtime_error("filename argument expected without quotes");
    return std::make_unique<Touch>(args[0].value);
}

CommandFactory::CommandPtr CommandFactory::createWc(ArgumentsVector args) {
    validateOptions("wc", args, std::unordered_set<std::string>{"w", "c"});
    if (args.size() > 2)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("argument and option not given");
    if (args.size() == 1) {
        if (args[0].type != TokenType::Option)
            throw std::runtime_error("option not given");
        auto mode = args[0].value == "w" ? Wc::Mode::Words : Wc::Mode::Chars;
        return std::make_unique<Wc>(std::nullopt, mode);
    }
    if (args[0].type != TokenType::Option || args[1].type == TokenType::Option) {
        throw std::runtime_error("expected an option followed by an argument");
    }
    Argument argument{args[1].value, args[1].type == TokenType::QuotedString};
    auto mode = args[0].value == "w" ? Wc::Mode::Words : Wc::Mode::Chars;
    return std::make_unique<Wc>(argument, mode);
}

CommandFactory::CommandPtr CommandFactory::createEcho(ArgumentsVector args) {
    validateOptions("echo", args, std::unordered_set<std::string>{});
    if (args.size() > 1)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.size() == 1) {
        Argument argument{args[0].value, args[0].type == TokenType::QuotedString};
        return std::make_unique<Echo>(argument);
    }
    return std::make_unique<Echo>(std::nullopt);
}

CommandFactory::CommandPtr CommandFactory::createBatch(ArgumentsVector args) {
    validateOptions("batch", args, std::unordered_set<std::string>{});
    if (args.size() > 1)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("argument not given");
    if (args[0].type != TokenType::Filename)
        throw std::runtime_error("filename argument expected without quotes");
    return std::make_unique<Batch>(args[0].value);
}

CommandFactory::CommandPtr CommandFactory::createPrompt(ArgumentsVector args) {
    validateOptions("prompt", args, std::unordered_set<std::string>{});
    if (args.size() > 1)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.size() == 1) {
        if (args[0].type != TokenType::QuotedString)
            throw std::runtime_error("expected a quoted string argument");
        return std::make_unique<Prompt>(args[0].value);
    }
    throw std::runtime_error("argument not given");
}

void CommandFactory::validateOptions(
    const std::string &commandName,
    ArgumentsVector args,
    const std::unordered_set<std::string> &options) {
    std::vector<std::string> notFound;
    for (auto& arg : args) {
        if (arg.type == TokenType::Option) {
            if (!options.contains(arg.value)) notFound.push_back(arg.value);
        }
    }
    if (!notFound.empty()) {
        throw InvalidOptionException(commandName, notFound);
    }
}

std::unique_ptr<Command> CommandFactory::create(const ParsedCommand& pc) {
    const auto& name = pc.name;
    const auto& tokens = pc.tokens;
    auto it = m_commandsMap.find(name);
    if (it == m_commandsMap.end())
        throw std::runtime_error("unknown command: " + name);
    return it->second(tokens);
}
