#ifndef CLI_INTERPRETER_H
#define CLI_INTERPRETER_H

#include <iostream>
#include <stack>

class Interpreter {
public:
    Interpreter(std::istream& in=std::cin, std::ostream& out=std::cout, char prompt='$');
    void run();
private:
    bool m_running = false;
    char m_promptChar;

    std::stack<std::istream*> m_inputStack;
    std::stack<std::ostream*> m_outputStack;
    std::ostream& m_errorStream = std::cerr;

    std::istream& getCurrentInput();
    std::ostream& getCurrentOutput();

    void printPrompt();

    void loop();

    std::string readLine();
};

#endif //CLI_INTERPRETER_H