#include "InvalidOptionsException.h"

InvalidOptionException::InvalidOptionException(
    const std::string &commandName,
    const std::vector<std::string> &invalidOptions) : std::runtime_error(fullMessage(commandName, invalidOptions)) {}

std::string InvalidOptionException::fullMessage(const std::string& commandName, const std::vector<std::string>& options) {
    std::string message = commandName + ": invalid options [";
    for (size_t i = 0; i < options.size(); ++i) {
        message += "-" + options[i] + (i == options.size() - 1 ? "]" : ", ");
    }
    return message;
}
