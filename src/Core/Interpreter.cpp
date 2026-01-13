#include <iostream>
#include <stdexcept>
#include "Interpreter.h"
#include "Command/CommandFactory.h"
#include "Parser/Parser.h"

Interpreter::Interpreter(std::istream& in,
                         std::ostream& out,
                         char prompt) : m_promptChar(prompt) {
    m_inputStack.push(&in);
    m_outputStack.push(&out);
}

void Interpreter::run() {
    m_running = true;
    loop();
}

void Interpreter::loop() {
    while (m_running) {
        printPrompt();
        std::string line = readLine();
        if (line.empty()) continue;
        try {
            auto parsed = Parser::parse(line);
            if (!parsed) continue;
            const auto command = CommandFactory::create(*parsed);
            command->execute(getCurrentInput(), getCurrentOutput());
        }
        catch (const std::exception& e) {
            std::ostream& out = getCurrentOutput();
            out << e.what() << std::endl;
        }
    }
}

std::string Interpreter::readLine() {
    std::istream& in = getCurrentInput();
    std::string line;
    if (!std::getline(in, line, '\n')) {
        if (m_inputStack.size() > 1) m_inputStack.pop();
        else m_running = false;
    }
    if (line.size() > 512)
        line.resize(512);

    return line;
}

std::istream& Interpreter::getCurrentInput() {
    if (m_inputStack.empty()) throw std::logic_error("Input stack empty");
    return *m_inputStack.top();
}

std::ostream& Interpreter::getCurrentOutput() {
    if (m_outputStack.empty()) throw std::logic_error("Output stack empty");
    return *m_outputStack.top();
}

void Interpreter::printPrompt() {
    std::ostream& out = getCurrentOutput();
    std::istream& in = getCurrentInput();
    if (&in == &std::cin) {
        out << m_promptChar << ' ';
    }
}