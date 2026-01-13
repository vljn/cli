#ifndef CLI_ECHO_H
#define CLI_ECHO_H

#include "../Core/Command/CommandWithInputArgument.h"


class Echo : public CommandWithInputArgument {
public:
    explicit Echo(const std::optional<Argument> &argument)
        : CommandWithInputArgument(argument) { }

    bool producesOutput() override { return true; }

    void do_execute(std::istream &in, std::ostream &out) override;
};


#endif //CLI_ECHO_H