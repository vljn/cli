#include <iostream>

#include "Core/Interpreter.h"

int main() {
    Interpreter interpreter(&std::cin, &std::cout);
    interpreter.run();
    return 0;
}