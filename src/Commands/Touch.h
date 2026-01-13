#ifndef CLI_TOUCH_H
#define CLI_TOUCH_H

#include <utility>

#include "../Core/Command.h"

class Touch : public Command {
public:
    Touch(std::string filename) : m_filename(std::move(filename)) {}

    bool consumesInput() override { return true; }

    bool producesOutput() override { return false; }

    void execute(const std::istream &in, std::ostream &out) override;

private:
    std::string m_filename;
};


#endif //CLI_TOUCH_H