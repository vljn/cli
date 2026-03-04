#include <unordered_set>
#include <algorithm>

#include "Core/CommandBase/Command.h"
#include "Core/Factory/CommandFactory.h"
#include "Commands/Batch.h"
#include "Commands/Date.h"
#include "Commands/Echo.h"
#include "Commands/Time.h"
#include "Commands/Touch.h"
#include "Commands/Wc.h"
#include "Commands/Prompt.h"
#include "Commands/Truncate.h"
#include "Commands/Rm.h"
#include "Commands/Head.h"
#include "Commands/Tr.h"
#include "Exceptions/InvalidOptionsException.h"

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
    },
    {
        "truncate", createTruncate
    },
    {
    "rm", createRm
    },
    {
    "head", createHead
    },
    {
    "tr", createTr
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
    if (args[0].type != TokenType::Word)
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
    if (args[0].type != TokenType::Word)
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

CommandFactory::CommandPtr CommandFactory::createTruncate(ArgumentsVector args) {
    validateOptions("truncate", args, std::unordered_set<std::string>{});
    if (args.size() > 1)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("argument not given");
    if (args[0].type != TokenType::Word)
        throw std::runtime_error("filename argument expected without quotes");
    return std::make_unique<Truncate>(args[0].value);
}

CommandFactory::CommandPtr CommandFactory::createRm(ArgumentsVector args) {
    validateOptions("rm", args, std::unordered_set<std::string>{});
    if (args.size() > 1)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("argument not given");
    if (args[0].type != TokenType::Word)
        throw std::runtime_error("filename argument expected without quotes");
    return std::make_unique<Rm>(args[0].value);
}

CommandFactory::CommandPtr CommandFactory::createHead(ArgumentsVector args) {
    if (args.size() > 2)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("number of lines not given (n<number> option expected)");
    if (args.size() == 1) {
        if (args[0].type != TokenType::Option)
            throw std::runtime_error("number of lines not given (n<number> option expected)");
    }
    if (args[0].type != TokenType::Option || args[1].type == TokenType::Option) {
        throw std::runtime_error("expected an option followed by an argument");
    }
    if (args[0].value.length() < 2 || args[0].value[0] != 'n') {
        throw std::runtime_error("count option expected with format n<number>");
    }
    if (args[0].value.length() > 6 || !std::ranges::all_of(args[0].value.begin() + 1, args[0].value.end(), ::isdigit)) {
        throw std::runtime_error("invalid count value");
    }
    unsigned count;
    try {
        count = std::stoul(args[0].value.substr(1));
    }
    catch (const std::exception& e) {
        throw std::runtime_error("invalid count value");
    }
    if (args.size() == 1) {
        return std::make_unique<Head>(std::nullopt, count);
    }
    Argument argument{args[1].value, args[1].type == TokenType::QuotedString};
    return std::make_unique<Head>(argument, count);
}

CommandFactory::CommandPtr CommandFactory::createTr(ArgumentsVector args) {
    if (args.size() > 3)
        throw std::runtime_error("number of arguments greater than expected");
    if (args.empty())
        throw std::runtime_error("what to replace not given (expected -what option)");
    if (args.size() == 1) {
        if (args[0].type != TokenType::Option)
            throw std::runtime_error("what to replace not given (expected -what option)");
        return std::make_unique<Tr>(std::nullopt, args[0].value, "");
    }
    if (args.size() == 2) {
        if (args[0].type == TokenType::Option) {
            if (args[1].type != TokenType::QuotedString) {
                throw std::runtime_error("-with string expected after -what option");
            }
            return std::make_unique<Tr>(std::nullopt, args[0].value, args[1].value);
        }
        if (args[1].type != TokenType::Option) {
            throw std::runtime_error("-what option expected after argument");
        }
        Argument argument{args[0].value, args[0].type == TokenType::QuotedString};
        return std::make_unique<Tr>(argument, args[0].value, "");
    }
    if (args[0].type == TokenType::Option || args[1].type != TokenType::Option || args[2].type == TokenType::Option || args[2].type == TokenType::Word) {
        throw std::runtime_error("expected an argument followed by -what option followed by -with string");
    }
    Argument argument{args[0].value, args[0].type == TokenType::QuotedString};
    return std::make_unique<Tr>(argument, args[1].value, args[2].value);
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
