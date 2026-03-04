#ifndef CLI_PARSEDCOMMAND_H
#define CLI_PARSEDCOMMAND_H

#include <string>
#include <vector>

enum class TokenType {
    Word,
    QuotedString,
    Option,
    RedirectIn,
    RedirectOutOverwrite,
    RedirectOutAppend,
    PipelineDivider
};

struct Token {
    std::string value;
    TokenType type;
    size_t position;
};

struct ParsedCommand {
    std::string name;
    std::vector<Token> tokens;
    std::string redirectInFilename;
    std::string redirectOutFilename;
    bool redirectOutAppend = false;
};

struct ParsedPipeline {
    std::vector<std::unique_ptr<ParsedCommand>> commands;
};

#endif //CLI_PARSEDCOMMAND_H