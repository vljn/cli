#ifndef CLI_COMMANDWITHINPUTARGUMENT_H
#define CLI_COMMANDWITHINPUTARGUMENT_H

#include <memory>
#include <optional>

#include "Command.h"

struct Argument {
    std::string value;
    bool quoted = false;
};

class CommandWithInputArgument : public Command {
public:
    CommandWithInputArgument() = delete;

    explicit CommandWithInputArgument(const std::optional<Argument>& argument);

    CommandResult execute(std::istream& in, std::ostream& out, std::ostream& err) final;

    virtual CommandResult do_execute(std::istream& in, std::ostream& out, std::ostream& err) = 0;

    bool consumesInput() final { return true; }
private:
    std::unique_ptr<std::istream> m_in;
};

#endif //CLI_COMMANDWITHINPUTARGUMENT_H