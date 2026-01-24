#ifndef CLI_TIME_H
#define CLI_TIME_H

#include "../Core/CommandBase/Command.h"

class Time : public Command {
public:
    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif //CLI_TIME_H