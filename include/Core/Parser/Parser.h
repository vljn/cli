#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <memory>
#include <string>

#include "ParsedCommand.h"

class Command;

class Parser {
public:
    static std::unique_ptr<ParsedCommand> parse(const std::string& line);
private:
    static bool isAllowed(char c);
    static void moveBuffer(std::string& buffer, ParsedCommand& pc, TokenType type);
    static size_t parseName(const std::string &line, size_t index, ParsedCommand &pc, std::vector<size_t> &errorPositions);
    static void parseElse(const std::string &line, size_t index, ParsedCommand &pc, std::vector<size_t> &errorPositions, size_t *quotesPosition, std::
                          vector<size_t> &invalidOptions);
    static size_t firstNonWhitespace(const std::string& line);
};

#endif //CLI_PARSER_H