#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include <iostream>
#include <memory>
#include <stack>
#include <vector>

struct ParsedCommand;
struct ParsedPipeline;

class Interpreter {
public:
    Interpreter(std::istream& in=std::cin, std::ostream& out=std::cout, std::ostream& errorOut=std::cerr, std::string prompt="$");
    void run();
private:
    const size_t MAX_LINE_LENGTH = 512;

    bool m_running = false;
    std::string m_promptString;

    std::stack<std::unique_ptr<std::istream>> m_inputStack;
    std::istream& m_inputStream;
    std::vector<std::unique_ptr<std::ostream>> m_outputStack;
    std::ostream& m_outputStream;
    std::ostream& m_errorStream;

    std::istream& getCurrentInput();
    std::ostream& getCurrentOutput() const;

    void printPrompt();

    void loop();

    void executePipeline(const ParsedPipeline& pipeline);

    static std::unique_ptr<std::istream> handleInRedirection(const ParsedCommand& parsedCommand);
    static std::unique_ptr<std::ostream> handleOutRedirection(const ParsedCommand& parsedCommand);

    std::string readLine();
};

#endif //CLI_INTERPRETER_H