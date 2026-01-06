#ifndef CLI_TIME_H
#define CLI_TIME_H

#include "../Core/Command.h"


class Time : public Command {
public:
    Time();

    void execute(std::istream* in, std::ostream* out) override;
};


#endif //CLI_TIME_H