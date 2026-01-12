#ifndef CLI_TIME_H
#define CLI_TIME_H

#include "../Core/Command.h"


class Time : public Command {
public:
    Time() = default;

    bool consumesInput() override { return false; }
    bool producesOutput() override { return true; }

    void execute(const std::istream& in, std::ostream& out) override;
};


#endif //CLI_TIME_H