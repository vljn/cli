#include <utility>
#include <memory>
#include <fstream>

#include "Core/Interpreter.h"


#include "Core/Factory/CommandFactory.h"
#include "Core/Parser/Parser.h"

Interpreter::Interpreter(std::istream& in,
                         std::ostream& out,
                         std::ostream& errorOut,
                         std::string prompt) : m_promptString(std::move(prompt)), m_inputStream(in), m_outputStream(out), m_errorStream(errorOut) { }

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
            Parser p(line);
            auto parsed = p.parse();
            if (!parsed) continue;
            const auto command = CommandFactory::create(*parsed);

            std::unique_ptr<std::istream> in = nullptr;
            std::unique_ptr<std::ostream> out = nullptr;
            if (!command->consumesInput() && !parsed->redirectInFilename.empty()) {
                throw std::runtime_error("redirection of input is not allowed for this command");
            }
            if (!parsed->redirectInFilename.empty())
                in = handleInRedirection(*parsed);
            if (!command->producesOutput() && !parsed->redirectOutFilename.empty()) {
                throw std::runtime_error("redirection of output is not allowed for this command");
            }
            if (!parsed->redirectOutFilename.empty())
                out = handleOutRedirection(*parsed);
            auto result = command->execute(in ? *in : m_inputStream, out ? *out : getCurrentOutput(), m_errorStream);

            if (result.action == InterpreterAction::PushStream) {
                if (result.newStream) {
                    m_inputStack.push(std::move(result.newStream));
                    if (out) {
                        m_outputStack.push_back(std::move(out));
                    } else {
                        m_outputStack.push_back(nullptr);
                    }
                }
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

std::unique_ptr<std::istream> Interpreter::handleInRedirection(const ParsedCommand& parsedCommand) {
        std::unique_ptr<std::istream> in = nullptr;

        if (!parsedCommand.redirectInFilename.empty()) {
            in = std::make_unique<std::ifstream>(parsedCommand.redirectInFilename);
            if (!*in) {
                throw std::runtime_error("error opening a file: " + parsedCommand.redirectInFilename);
            }
        }

        return std::move(in);
}

std::unique_ptr<std::ostream> Interpreter::handleOutRedirection(const ParsedCommand& parsedCommand) {
    std::unique_ptr<std::ostream> out = nullptr;

    if (!parsedCommand.redirectOutFilename.empty()) {
        if (parsedCommand.redirectOutAppend) {
            out = std::make_unique<std::ofstream>(parsedCommand.redirectOutFilename, std::ios_base::app);
        }
        else {
            out = std::make_unique<std::ofstream>(parsedCommand.redirectOutFilename);
        }
        if (!*out) {
            throw std::runtime_error("error opening a file: " + parsedCommand.redirectOutFilename);
        }
    }

    return std::move(out);
}

std::string Interpreter::readLine() {
    std::istream& in = getCurrentInput();
    std::string line;
    if (!std::getline(in, line, '\n')) {
        if (!m_inputStack.empty()) {
            m_inputStack.pop();
        }
        else m_running = false;
    }
    if (line.size() > MAX_LINE_LENGTH)
        line.resize(MAX_LINE_LENGTH);

    return line;
}

std::istream& Interpreter::getCurrentInput() {
    if (m_inputStack.empty()) return m_inputStream;
    return *m_inputStack.top();
}

std::ostream& Interpreter::getCurrentOutput() {
    if (m_outputStack.empty()) return m_outputStream;
    for (int i = m_outputStack.size() - 1; i >= 0; --i) {
        if (m_outputStack[i]) return *m_outputStack[i];
    }
    return m_outputStream;
}

void Interpreter::printPrompt() {
    const std::istream& in = getCurrentInput();
    if (&in == &m_inputStream) {
        m_outputStream << m_promptString << ' ';
    }
}
