#ifndef CLI_HEAD_H
#define CLI_HEAD_H

#include "Core/CommandBase/CommandWithInputArgument.h"

class Head : public CommandWithInputArgument {
public:
    explicit Head(const std::optional<Argument>& argument, const unsigned lineCount)
        : CommandWithInputArgument(argument), m_lineCount(lineCount) { }

    CommandResult do_execute(std::istream& in, std::ostream& out, std::ostream& err) override;
private:
    unsigned m_lineCount;
};

#endif //CLI_HEAD_H