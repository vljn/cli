#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <memory>
#include <string>

#include "ParsedCommand.h"

class Command;

class Parser {
public:
    static std::unique_ptr<ParsedCommand> parse(std::string& line);
private:
    static bool isAllowed(char c);
    static void trim(std::string& s);
    static void moveBuffer(std::string& buffer, ParsedCommand& pc, TokenType type);
    static unsigned parseName(std::string& line, ParsedCommand& pc);
    static void parseElse(std::string& line, unsigned index, ParsedCommand& pc);
};

#endif //CLI_PARSER_H