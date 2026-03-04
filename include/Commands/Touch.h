#ifndef CLI_TOUCH_H
#define CLI_TOUCH_H

#include <utility>

#include "Core/CommandBase/Command.h"

class Touch : public Command {
public:
    explicit Touch(std::string filename) : m_filename(std::move(filename)) {}

    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;

    bool consumesInput() override { return false; }
    bool producesOutput() override { return false; }
private:
    std::string m_filename;
};

#endif //CLI_TOUCH_H