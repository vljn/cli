#ifndef CLI_TOUCH_H
#define CLI_TOUCH_H

#include <utility>

#include "../Core/CommandBase/Command.h"

class Touch : public Command {
public:
    Touch(std::string filename) : m_filename(std::move(filename)) {}

    bool consumesInput() override { return true; }

    bool producesOutput() override { return false; }

    void execute(std::istream &in, std::ostream &out, std::ostream& err) override;

private:
    std::string m_filename;
};

#endif //CLI_TOUCH_H