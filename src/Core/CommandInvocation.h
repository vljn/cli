#ifndef CLI_COMMANDINVOCATION_H
#define CLI_COMMANDINVOCATION_H

#include <vector>

struct Argument {
    std::string value;
    std::string name;
    bool literal;
};

class CommandInvocation {
public:
    std::vector<Argument> args;
};


#endif //CLI_COMMANDINVOCATION_H