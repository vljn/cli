#include <utility>
#include <memory>
#include <fstream>
#include <sstream>

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

            executePipeline(*parsed);
        }
        catch (const std::exception& e) {
            m_errorStream << "Error - " << e.what() << std::endl;
        }
    }
}

void Interpreter::executePipeline(const ParsedPipeline& pipeline) {
    if (pipeline.commands.empty()) return;

    std::unique_ptr<std::stringstream> previousOutput = nullptr;

    bool lastProducesOutput = false;

    for (int i = 0; i < pipeline.commands.size(); ++i) {
        const auto& parsed = *pipeline.commands[i];
        const auto commandPtr = CommandFactory::create(parsed);

        bool isFirst = (i == 0);
        bool isLast = (i == pipeline.commands.size() - 1);

        std::istream* finalIn = nullptr;
        std::unique_ptr<std::istream> fileIn = nullptr;
        if (isFirst) {
            if (!parsed.redirectInFilename.empty()) {
                if (!commandPtr->consumesInput()) {
                    throw std::runtime_error("redirection of input is not allowed for this command");
                }
                fileIn = handleInRedirection(parsed);
                finalIn = fileIn.get();
            }
            else {
                finalIn = &m_inputStream;
            }
        }
        else {
            finalIn = previousOutput.get();
        }

        std::ostream* finalOut = nullptr;
        std::unique_ptr<std::ostream> fileOut = nullptr;
        std::unique_ptr<std::stringstream> nextOutput = nullptr;
        if (isLast) {
            if (!parsed.redirectOutFilename.empty()) {
                if (!commandPtr->producesOutput())
                    throw std::runtime_error("redirection of output is not allowed for this command");
                fileOut = handleOutRedirection(parsed);
                finalOut = fileOut.get();
            } else {
                finalOut = &getCurrentOutput();
            }
        } else {
            nextOutput = std::make_unique<std::stringstream>();
            finalOut = nextOutput.get();
        }


        auto result = commandPtr->execute(*finalIn, *finalOut, m_errorStream);

        previousOutput = std::move(nextOutput);

        lastProducesOutput = commandPtr->producesOutput();

        if (isLast) {
            if (result.action == InterpreterAction::PushStream) {
                if (result.newStream) {
                    m_inputStack.push(std::move(result.newStream));

                    if (fileOut) {
                        m_outputStack.push_back(std::move(fileOut));
                    } else {
                        m_outputStack.push_back(nullptr);
                    }
                }
            }
        }
        if (result.action == InterpreterAction::SetPromptString) {
            m_promptString = result.newPromptString;
        }
    }

    auto lastCommand = *pipeline.commands.back();
    if (lastProducesOutput && lastCommand.name != "batch" && lastCommand.redirectOutFilename.empty()) {
        getCurrentOutput() << std::endl;
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
            if (!m_outputStack.empty()) {
                m_outputStack.pop_back();
            }

            return "";
        }
        m_running = false;
        return "";
    }
    if (line.size() > MAX_LINE_LENGTH)
        line.resize(MAX_LINE_LENGTH);

    return line;
}

std::istream& Interpreter::getCurrentInput() {
    if (m_inputStack.empty()) return m_inputStream;
    return *m_inputStack.top();
}

std::ostream& Interpreter::getCurrentOutput() const {
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
