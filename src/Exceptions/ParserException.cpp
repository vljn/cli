#include "ParserException.h"

ParserException::ParserException(const std::string& line, const std::vector<size_t>& errorPositions,
    const std::string &message) : std::runtime_error(message) {
    std::string errorLine(line.length(), ' ');
    if (errorPositions.empty()) {
        fullMessage = message;
        return;
    }

    for (auto i : errorPositions) {
        if (i < errorLine.length())
            errorLine[i] = '^';
    }

    fullMessage = message + "\n" + line + "\n" + errorLine;
}

const char * ParserException::what() const noexcept {
    return fullMessage.c_str();
}
