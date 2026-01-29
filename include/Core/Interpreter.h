#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include <iostream>
#include <stack>

class Interpreter {
public:
    Interpreter(std::istream& in=std::cin, std::ostream& out=std::cout, std::ostream& errorOut=std::cerr, std::string prompt="$");
    void run();
private:
    bool m_running = false;
    std::string m_promptString;

    std::stack<std::istream*> m_inputStack;
    std::istream& m_inputStream;
    std::ostream& m_outputStream;
    std::ostream& m_errorStream;

    std::istream& getCurrentInput();

    void printPrompt();

    void loop();

    std::string readLine();
};

#endif //CLI_INTERPRETER_H