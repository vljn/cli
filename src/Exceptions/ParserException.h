#ifndef CLI_PARSEREXCEPTION_H
#define CLI_PARSEREXCEPTION_H

#include <stdexcept>
#include <vector>

class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& line, const std::vector<size_t>& errorPositions, const std::string& message);
    const char * what() const noexcept override;
private:
    std::string fullMessage;
};

#endif //CLI_PARSEREXCEPTION_H