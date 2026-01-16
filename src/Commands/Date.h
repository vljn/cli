#ifndef CLI_DATE_H
#define CLI_DATE_H

#include "../Core/CommandBase/Command.h"

class Date : public Command {
public:
    void execute(std::istream &in, std::ostream &out, std::ostream& err) override;
};

#endif //CLI_DATE_H