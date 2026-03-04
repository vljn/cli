#ifndef CLI_DATE_H
#define CLI_DATE_H

#include "Core/CommandBase/Command.h"

class Date : public Command {
public:
    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;

    bool consumesInput() override { return false; }
    bool producesOutput() override { return true; }
};

#endif //CLI_DATE_H