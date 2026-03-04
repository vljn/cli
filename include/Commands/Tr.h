#ifndef CLI_TR_H
#define CLI_TR_H

#include <utility>

#include "Core/CommandBase/CommandWithInputArgument.h"

class Tr : public CommandWithInputArgument {
public:
    Tr(const std::optional<Argument> &argument, std::string what, std::string with)
        : CommandWithInputArgument(argument), m_what(std::move(what)), m_with(std::move(with))  { }

    CommandResult do_execute(std::istream& in, std::ostream& out, std::ostream& err) override;
private:
    std::string m_what;
    std::string m_with;
};


#endif //CLI_TR_H