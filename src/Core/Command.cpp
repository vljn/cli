#include "Command.h"
#include "Parser/ParsedCommand.h"

Command::Command(CommandInvocation invocation) {
    for (auto i : invocation.args) {
        args.push_back(i);
    }
}
