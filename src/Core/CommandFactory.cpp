#include "CommandFactory.h"
#include "Command.h"
#include "../Commands/Date.h"
#include "../Commands/Time.h"
#include "../Commands/Touch.h"

std::unique_ptr<Command> CommandFactory::create(const ParsedCommand& pc) {
    if (pc.name == "time") {
        return std::make_unique<Time>();
    }
    if (pc.name == "date") {
        return std::make_unique<Date>();
    }
    if (pc.name == "touch") {
        if (pc.tokens.size() != 1)
            throw std::runtime_error("Number of arguments greater than expected");
        if (pc.tokens[0].type != TokenType::Filename)
            throw std::runtime_error("Filename argument expected without quotes");
        return std::make_unique<Touch>(pc.tokens[0].value);
    }
    throw std::runtime_error("Command does not exist");
}
