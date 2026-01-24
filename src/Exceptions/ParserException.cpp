#include "Exceptions/ParserException.h"

ParserException::ParserException(const std::string& line, const std::vector<size_t>& errorPositions,
                                 const std::string &message) : std::runtime_error(fullMessage(line, errorPositions, message)) {

}

std::string ParserException::fullMessage(
    const std::string& line,
    const std::vector<size_t>& errorPositions,
    const std::string &message) {
    std::string errorLine(line.length(), ' ');
    if (errorPositions.empty()) {
        return message;
    }

    for (auto i : errorPositions) {
        if (i < errorLine.length())
            errorLine[i] = '^';
    }

    return message + "\n" + line + "\n" + errorLine;
}
