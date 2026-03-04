#ifndef CLI_PARSEDCOMMAND_H
#define CLI_PARSEDCOMMAND_H

#include <string>
#include <vector>

enum class TokenType {
    Word,
    QuotedString,
    Option
};

struct Token {
    std::string value;
    TokenType type;
    size_t position;
};

struct ParsedCommand {
    std::string name;
    std::vector<Token> tokens;
};

#endif //CLI_PARSEDCOMMAND_H