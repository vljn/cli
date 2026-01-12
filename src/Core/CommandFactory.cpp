#include "CommandFactory.h"
#include "Command.h"
#include "../Commands/Time.h"

std::unique_ptr<Command> CommandFactory::create(const ParsedCommand& pc) {
    if (pc.name == "time") {
        return std::make_unique<Time>();
    }
    throw std::runtime_error("Command does not exist");
}
