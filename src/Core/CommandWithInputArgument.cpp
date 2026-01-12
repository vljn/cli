#include <sstream>
#include <fstream>
#include "CommandWithInputArgument.h"


CommandWithInputArgument::CommandWithInputArgument(const std::optional<Argument>& argument) {
    if (!argument) return;

    if (argument.value().quoted) {
        m_in = std::make_unique<std::istringstream>(argument.value().value);
        return;
    }

    auto file = std::make_unique<std::ifstream>(argument.value().value);
    if (!*file) {
        throw std::runtime_error("Error opening a file: " + argument.value().value);
    }
    m_in = std::move(file);
}

void CommandWithInputArgument::execute(const std::istream& in, std::ostream& out) {
    if (m_in) do_execute(*m_in, out);
    else do_execute(in, out);
}