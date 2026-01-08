#include <string>
#include <iostream>
#include <stdexcept>
#include <cctype>
#include "Parser.h"
#include "ParsedCommand.h"

ParsedCommand* Parser::parse(std::string& line) {
    trim(line);
    if (line.empty()) return nullptr;
    auto parsed = new ParsedCommand();
    unsigned index = parseName(line, *parsed);
    parseElse(line, index, *parsed);
    return parsed;
}

bool Parser::isAllowed(char c) {
    return std::isalnum(c);
}

void Parser::trim(std::string& s) {
    auto first = s.find_first_not_of(" \t");
    if (first == std::string::npos) {
        s.clear();
        return;
    }
    auto last = s.find_last_not_of(" \t");
    s = s.substr(first, (last - first + 1));
}

void Parser::moveBuffer(std::string& buffer, ParsedCommand& pc, TokenType type) {
    pc.tokens.push_back(Token{buffer, type});
    buffer = "";
}

unsigned Parser::parseName(std::string& line, ParsedCommand& pc) {
    unsigned i;
    std::string buffer;
    for (i = 0; i < line.length(); i++) {
        if (isspace(line[i])) {
            pc.name = buffer;
            return i;
        }
        if (isAllowed(line[i])) {
            buffer += line[i];
            continue;
        }
        throw std::runtime_error("Character not allowed");
    }
    pc.name = buffer;
    return i;
}

void Parser::parseElse(std::string& line, unsigned index, ParsedCommand& pc) {
    std::string buffer;
    unsigned i;
    bool inQuotes = false;
    bool isOption = false;
    bool prevSpace = true;
    for (i = index + 1; i < line.length(); i++) {
        if (line[i] == '"') {
            if (inQuotes) {
                moveBuffer(buffer, pc, isOption ? TokenType::Option : TokenType::QuotedString);
                inQuotes = false;
                isOption = false;
            }
            else {
                inQuotes = true;
            }
            continue;
        }
        if (inQuotes) {
            buffer += line[i];
            continue;
        }
        if (line[i] == '-') {
            if (isOption or !prevSpace) {
                throw std::runtime_error("Invalid option");
            }
            isOption = true;
            prevSpace = false;
            continue;
        }
        if (isspace(line[i])) {
            if (prevSpace) {
                continue;
            }
            if (isOption and buffer.empty()) throw std::runtime_error("Invalid option");
            moveBuffer(buffer, pc, isOption ? TokenType::Option : TokenType::Filename);
            prevSpace = true;
            isOption = false;
            continue;
        }
        if (isAllowed(line[i]) or (!isOption and (line[i] == '.' or line[i] == '-' or line[i] == '_'))) {
            buffer += line[i];
        }
        else {
            throw std::runtime_error("Character not allowed");
        }
        prevSpace = false;
    }
    if (inQuotes) {
        throw std::runtime_error("Quotes error");
    }
    if (!buffer.empty()) {
        moveBuffer(buffer, pc, isOption ? TokenType::Option :TokenType::Filename);
        isOption = false;
    }
    if (isOption)
        throw std::runtime_error("Invalid option");
}