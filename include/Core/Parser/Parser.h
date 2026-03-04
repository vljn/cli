#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <memory>
#include <string>

#include "ParsedCommand.h"

class Command;

class Parser {
public:
    explicit Parser(const std::string& line) : line(line) {}

    std::unique_ptr<ParsedPipeline> parse();
private:
    std::vector<Token> tokenize();
    Token readWord(size_t& index, std::vector<size_t>& invalidPositions) const;
    Token readQuotedString(size_t& index);
    Token readOption(size_t& index, std::vector<size_t>& invalidPositions);

    static std::unique_ptr<ParsedCommand> buildSingle(const std::vector<Token>& tokens);
    std::unique_ptr<ParsedPipeline> buildPipeline(const std::vector<Token>& tokens);

    static bool isAllowed(const char c) {
        return std::isalnum(c) || c == '_' || c == '.' || c == '/' || c == '\\';
    }

    const std::string& line;
};

#endif //CLI_PARSER_H