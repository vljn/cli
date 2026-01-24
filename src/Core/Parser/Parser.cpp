#include <string>
#include <iostream>
#include <cctype>
#include <memory>

#include "Parser.h"
#include "ParsedCommand.h"
#include "../../Exceptions/ParserException.h"

std::unique_ptr<ParsedCommand> Parser::parse(std::string& line) {
    auto firstChar = firstNonWhitespace(line);
    if (firstChar == std::string::npos) return nullptr;
    std::vector<size_t> unexpectedCharacters;
    size_t quotesError = std::string::npos;
    std::vector<size_t> invalidOptions;
    auto parsed = std::make_unique<ParsedCommand>();
    size_t index = parseName(line, firstChar, *parsed, unexpectedCharacters);
    parseElse(line, index, *parsed, unexpectedCharacters, &quotesError, invalidOptions);
    if (!unexpectedCharacters.empty()) {
        throw ParserException(line, unexpectedCharacters, "Unexpected characters: ");
    }
    if (quotesError != std::string::npos) {
        throw ParserException(line, {quotesError}, "Quotes not closed");
    }
    if (!invalidOptions.empty()) {
        throw ParserException(line, invalidOptions, "Option not specified");
    }
    return parsed;
}

bool Parser::isAllowed(char c) {
    return std::isalnum(c);
}

size_t Parser::firstNonWhitespace(const std::string &line) {
    for (size_t i = 0; i < line.length(); i++) {
        if (!isspace(line[i])) return i;
    }
    return std::string::npos;
}

void Parser::moveBuffer(std::string& buffer, ParsedCommand& pc, TokenType type) {
    if (!buffer.empty())
        pc.tokens.push_back(Token{buffer, type});
    buffer = "";
}

size_t Parser::parseName(std::string &line, size_t index, ParsedCommand &pc, std::vector<size_t> &errorPositions) {
    size_t i;
    std::string buffer;
    for (i = index; i < line.length(); i++) {
        if (isspace(line[i])) {
            pc.name = buffer;
            return i;
        }
        if (isAllowed(line[i])) {
            buffer += line[i];
            continue;
        }
        errorPositions.push_back(i);
    }
    pc.name = buffer;
    return i;
}

void Parser::parseElse(std::string& line, size_t index, ParsedCommand& pc, std::vector<size_t>& errorPositions, size_t* quotesPosition, std::vector<size_t>& invalidOptions) {
    std::string buffer;
    size_t i;
    bool inQuotes = false;
    bool isOption = false;
    bool prevSpace = true;
    for (i = index; i < line.length(); i++) {
        char c = line[i];

        if (c == '"') {
            if (inQuotes) {
                moveBuffer(buffer, pc, isOption ? TokenType::Option : TokenType::QuotedString);
                inQuotes = false;
                isOption = false;
                *quotesPosition = std::string::npos;
            }
            else {
                *quotesPosition = i;
                inQuotes = true;
            }
            prevSpace = false;
            continue;
        }
        if (inQuotes) {
            buffer += c;
            continue;
        }
        if (c == '-') {
            if (!prevSpace) {
                buffer += c;
                continue;
            }
            isOption = true;
            prevSpace = false;
            continue;
        }
        if (isspace(c)) {
            if (prevSpace) {
                continue;
            }
            if (isOption and buffer.empty()) {
                invalidOptions.push_back(i);
                prevSpace = true;
                isOption = false;
                continue;
            }
            moveBuffer(buffer, pc, isOption ? TokenType::Option : TokenType::Filename);
            prevSpace = true;
            isOption = false;
            continue;
        }
        if (isAllowed(c) or c == '.' or c == '_' or c == '/') {
            buffer += c;
        }
        else {
            errorPositions.push_back(i);
        }
        prevSpace = false;
    }
    if (!buffer.empty()) {
        moveBuffer(buffer, pc, isOption ? TokenType::Option :TokenType::Filename);
        isOption = false;
    }
    if (isOption)
        invalidOptions.push_back(line.length() - 1);
}