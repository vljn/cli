#ifndef CLI_DATE_H
#define CLI_DATE_H

#include "../Core/Command.h"


class Date : public Command {
public:
    bool consumesInput() override { return false; }
    bool producesOutput() override { return true; }
    void execute(std::istream &in, std::ostream &out) override;
};


#endif //CLI_DATE_H