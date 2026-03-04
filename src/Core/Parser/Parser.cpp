 #include <string>
#include <iostream>
#include <cctype>
#include <memory>

#include "Core/Parser/Parser.h"
#include "Core/Parser/ParsedCommand.h"
#include "Exceptions/ParserException.h"

std::unique_ptr<ParsedCommand> Parser::parse() {
    std::vector<Token> tokens = tokenize();
    if (tokens.empty()) return nullptr;
    return build(tokens);
}

std::vector<Token> Parser::tokenize() {
    std::vector<Token> tokens;
    std::vector<size_t> invalidCharacterPositions;
    size_t i = 0;

    while (i < line.length()) {
        const char c = line[i];

        if (std::isspace(c)) {
            i++;
            continue;
        }

        if (c == '"') {
            tokens.push_back(readQuotedString(i));
        } else if (c == '-') {
            tokens.push_back(readOption(i, invalidCharacterPositions));
        } else if (c == '<') {
            tokens.push_back({"<", TokenType::RedirectIn, i++});
        }
        else if (c == '>') {
            if (i + 1 < line.length() && line[i + 1] == '>') {
                tokens.push_back({">>", TokenType::RedirectOutAppend, i});
                i += 2;
            } else {
                tokens.push_back({">", TokenType::RedirectOutOverwrite, i++});
            }
        } else {
            tokens.push_back(readWord(i, invalidCharacterPositions));
        }
    }

    if (!invalidCharacterPositions.empty()) {
        throw ParserException(line, invalidCharacterPositions, "unexpected characters");
    }

    return tokens;
}

Token Parser::readWord(size_t& index, std::vector<size_t>& invalidPositions) const {
    std::string word;
    const auto start = index;
    while (index < line.length() && !std::isspace(line[index]) && line[index] != '"' && line[index] != '-' && line[index] != '<' && line[index] != '>') {
        const char c = line[index];
        if (!isAllowed(c)) {
            invalidPositions.push_back(index);
        }
        else {
            word += c;
        }
        index++;
    }

    return {word, TokenType::Word, start};
}


Token Parser::readQuotedString(size_t& index) {
    std::string str;
    const auto start = index;
    index++;
    while (index < line.length() && line[index] != '"') {
        const char c = line[index];
        str += c;
        index++;
    }
    if (index == line.length()) {
        throw ParserException(line, {start}, "quotes not closed");
    }
    index++;
    return {str, TokenType::QuotedString, start};
}

Token Parser::readOption(size_t& index, std::vector<size_t>& invalidPositions) {
    std::string option;
    const auto start = index;
    index++;
    if (index == line.length()) {
        throw ParserException(line, {start}, "option not specified");
    }

    if (line[index] == '"') {
        auto token = readQuotedString(index);
        token.type = TokenType::Option;
        token.position = start;
        return token;
    }

    while (index < line.length() && !std::isspace(line[index]) && line[index] != '<' && line[index] != '>') {
        const char c = line[index];
        if (c == '"' || c == '-') {
            invalidPositions.push_back(index);
            index++;
            continue;
        }
        if (!isAllowed(c)) {
            invalidPositions.push_back(index);
        }
        else {
            option += c;
        }
        index++;
    }

    return {option, TokenType::Option, start};
}

std::unique_ptr<ParsedCommand> Parser::build(const std::vector<Token>& tokens) {
    auto parsedCommand = std::make_unique<ParsedCommand>();

    if (tokens[0].type != TokenType::Word) {
        throw std::runtime_error("command name not given");
    }
    parsedCommand->name = tokens[0].value;


    bool hasRedirectIn = false;
    bool hasRedirectOut = false;
    for (size_t i = 1; i < tokens.size(); i++) {
        const auto& token = tokens[i];

        if (token.type == TokenType::RedirectIn || token.type == TokenType::RedirectOutOverwrite || token.type == TokenType::RedirectOutAppend) {
            if (i + 1 >= tokens.size()) {
                throw std::runtime_error("redirection filename not specified");
            }

            const auto& nextToken = tokens[i + 1];
            if (nextToken.type != TokenType::Word) {
                throw std::runtime_error("file name expected after redirection operator");
            }

            if (token.type == TokenType::RedirectIn) {
                if (hasRedirectIn) {
                    throw std::runtime_error("multiple input redirections specified");
                }
                hasRedirectIn = true;
                parsedCommand->redirectInFilename = nextToken.value;
            }
            else if (token.type == TokenType::RedirectOutOverwrite || token.type == TokenType::RedirectOutAppend) {
                if (hasRedirectOut) {
                    throw std::runtime_error("multiple output redirections specified");
                }
                hasRedirectOut = true;
                parsedCommand->redirectOutFilename = nextToken.value;
                parsedCommand->redirectOutAppend = token.type == TokenType::RedirectOutAppend;
            }
            i++;
        }
        else {
            if (hasRedirectIn || hasRedirectOut) {
                throw std::runtime_error("redirection expected at the end of the command");
            }
            parsedCommand->tokens.push_back(token);
        }
    }

    return parsedCommand;
}