#include <iostream>
#include <stdexcept>
#include <utility>

#include "Core/Interpreter.h"
#include "Core/Factory/CommandFactory.h"
#include "Core/Parser/Parser.h"

Interpreter::Interpreter(std::istream& in,
                         std::ostream& out,
                         std::ostream& errorOut,
                         std::string prompt) : m_promptString(std::move(prompt)), m_inputStream(in), m_outputStream(out), m_errorStream(errorOut) {
    m_inputStack.push(&std::cin);
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
            auto result = command->execute(m_inputStream, m_outputStream, m_errorStream);
            if (result.action == InterpreterAction::PushStream) {
                m_inputStack.push(result.newStream);
            }
            else if (result.action == InterpreterAction::SetPromptString) {
                m_promptString = result.newPromptString;
            }
        }
        catch (const std::exception& e) {
            m_errorStream << "Error - " << e.what() << std::endl;
        }
    }
}

std::string Interpreter::readLine() {
    std::istream& in = getCurrentInput();
    std::string line;
    if (!std::getline(in, line, '\n')) {
        if (m_inputStack.size() > 1) {
            delete m_inputStack.top();
            m_inputStack.pop();
        }
        else m_running = false;
    }
    if (line.size() > MAX_LINE_LENGTH)
        line.resize(MAX_LINE_LENGTH);

    return line;
}

std::istream& Interpreter::getCurrentInput() {
    if (m_inputStack.empty()) throw std::logic_error("Input stack empty");
    return *m_inputStack.top();
}

void Interpreter::printPrompt() {
    const std::istream& in = getCurrentInput();
    if (&in == &std::cin) {
        m_outputStream << m_promptString << ' ';
    }
}