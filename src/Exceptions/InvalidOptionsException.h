#ifndef CLI_INVALIDOPTIONSEXCEPTION_H
#define CLI_INVALIDOPTIONSEXCEPTION_H

#include <stdexcept>
#include <vector>
#include <string>

class InvalidOptionException : public std::runtime_error {
public:
    InvalidOptionException(const std::string& commandName, const std::vector<std::string>& invalidOptions);

private:
    static std::string fullMessage(
        const std::string &commandName,
        const std::vector<std::string> &options);
};

#endif //CLI_INVALIDOPTIONSEXCEPTION_H