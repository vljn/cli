#ifndef CLI_TRUNCATE_H
#define CLI_TRUNCATE_H

#include "../Core/CommandBase/Command.h"

class Truncate : public Command {
public:
    explicit Truncate(std::string filename) : m_filename(std::move(filename)) { }

    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) override;
private:
    std::string m_filename;
};

#endif //CLI_TRUNCATE_H