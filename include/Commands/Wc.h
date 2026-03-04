#ifndef CLI_WC_H
#define CLI_WC_H

#include "Core/CommandBase/CommandWithInputArgument.h"

class Wc : public CommandWithInputArgument {
public:
    enum class Mode {
        Chars,
        Words
    };

    explicit Wc(const std::optional<Argument> &argument, const Mode mode)
        : CommandWithInputArgument(argument), m_mode(mode) { }

    CommandResult do_execute(std::istream& in, std::ostream& out, std::ostream& err) override;

    bool producesOutput() override { return true; }
private:
    Mode m_mode;
};

#endif //CLI_WC_H