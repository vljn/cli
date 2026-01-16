#ifndef CLI_PARSEREXCEPTION_H
#define CLI_PARSEREXCEPTION_H

#include <stdexcept>
#include <vector>

class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& line, const std::vector<size_t>& errorPositions, const std::string& message);
private:
    static std::string fullMessage(
        const std::string &line,
        const std::vector<size_t> &errorPositions,
        const std::string &message);
};

#endif //CLI_PARSEREXCEPTION_H