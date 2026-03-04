#ifndef CLI_RM_H
#define CLI_RM_H

#include <utility>

#include "Core/CommandBase/Command.h"

class Rm : public Command {
public:
    explicit Rm(std::string filename) : m_filename(std::move(filename)) { }

    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;

private:
    std::string m_filename;
};

#endif //CLI_RM_H