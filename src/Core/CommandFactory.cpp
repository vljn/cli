#include "CommandFactory.h"
#include "Command.h"
#include "../Commands/Date.h"
#include "../Commands/Time.h"

std::unique_ptr<Command> CommandFactory::create(const ParsedCommand& pc) {
    if (pc.name == "time") {
        return std::make_unique<Time>();
    }
    if (pc.name == "date") {
        return std::make_unique<Date>();
    }
    throw std::runtime_error("Command does not exist");
}
