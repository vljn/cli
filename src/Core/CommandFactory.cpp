#include "CommandFactory.h"
#include "Command.h"
#include "../Commands/Date.h"
#include "../Commands/Echo.h"
#include "../Commands/Time.h"
#include "../Commands/Touch.h"
#include "../Commands/Wc.h"

std::unique_ptr<Command> CommandFactory::create(const ParsedCommand& pc) {
    const auto& name = pc.name;
    const auto& tokens = pc.tokens;
    if (name == "time") {
        return std::make_unique<Time>();
    }
    if (name == "date") {
        return std::make_unique<Date>();
    }
    if (name == "touch") {
        if (tokens.size() > 1)
            throw std::runtime_error("Number of arguments greater than expected");
        if (tokens.empty())
            throw std::runtime_error("Argument not given");
        if (tokens[0].type != TokenType::Filename)
            throw std::runtime_error("Filename argument expected without quotes");
        return std::make_unique<Touch>(tokens[0].value);
    }
    if (name == "wc") {
        if (tokens.size() > 2)
            throw std::runtime_error("Number of arguments greater than expected");
        if (tokens.empty())
            throw std::runtime_error("Argument and option not given");
        if (tokens.size() == 1) {
            if (tokens[0].type != TokenType::Option)
                throw std::runtime_error("Option not given");
            if (tokens[0].value != "w" and tokens[0].value != "c")
                throw std::runtime_error("Invalid option: expected '-w' or '-c'");
            auto mode = tokens[0].value == "w" ? Wc::Mode::Words : Wc::Mode::Chars;
            return std::make_unique<Wc>(std::nullopt, mode);
        }
        if (tokens[0].type != TokenType::Option || tokens[1].type == TokenType::Option) {
            throw std::runtime_error("Expected an option followed by an argument");
        }
        if (tokens[0].value != "w" and tokens[0].value != "c")
            throw std::runtime_error("Invalid option: expected '-w' or '-c'");
        Argument argument{tokens[1].value, tokens[1].type == TokenType::QuotedString};
        auto mode = tokens[0].value == "w" ? Wc::Mode::Words : Wc::Mode::Chars;
        return std::make_unique<Wc>(argument, mode);
    }
    if (name == "echo") {
        if (tokens.size() > 1)
            throw std::runtime_error("Number of arguments greater than expected");
        if (tokens.size() == 1) {
            if (tokens[0].type == TokenType::Option)
                throw std::runtime_error("Argument not given");
            Argument argument{tokens[0].value, tokens[0].type == TokenType::QuotedString};
            return std::make_unique<Echo>(argument);
        }
        return std::make_unique<Echo>(std::nullopt);
    }
    throw std::runtime_error("Command does not exist");
}
