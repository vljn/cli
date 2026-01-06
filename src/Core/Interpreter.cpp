#include <iostream>
#include <stdexcept>
#include "Interpreter.h"
#include "CommandFactory.h"
#include "Parser/ParsedCommand.h"
#include "Parser/Parser.h"

Interpreter::Interpreter(std::istream* in,
                         std::ostream* out,
                         char prompt) : m_promptChar(prompt) {
    m_inputStack.push(in);
    m_outputStack.push(out);
}

void Interpreter::run() {
    m_running = true;
    loop();
}

void Interpreter::loop() {
    while (m_running) {
        printPrompt();
        std::string line = readLine();
        ParsedCommand parsed = Parser::parse(line);
        auto command = CommandFactory::create(parsed);
        command->execute(getCurrentInput(), getCurrentOutput());
    }
}

std::string Interpreter::readLine() {
    std::istream* in = getCurrentInput();
    if (!in) {
        throw std::runtime_error("Interpreter: no current input stream");
    }

    std::string line;
    if (!std::getline(*in, line, '\n')) {
        if (m_inputStack.size() > 1) m_inputStack.pop();
        else m_running = false;
    }
    if (line.size() > 512)
        line.resize(512);

    return line;
}

std::istream* Interpreter::getCurrentInput() {
    if (!m_inputStack.empty()) return m_inputStack.top();
    return nullptr;
}

std::ostream* Interpreter::getCurrentOutput() {
    if (!m_outputStack.empty()) return m_outputStack.top();
    return nullptr;
}

void Interpreter::printPrompt() {
    std::ostream* out = getCurrentOutput();
    std::istream* in = getCurrentInput();
    if (in == &std::cin) {
        *out << m_promptChar << ' ';
    }
}