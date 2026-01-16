#include <sstream>
#include <fstream>
#include <filesystem>

#include "CommandWithInputArgument.h"

CommandWithInputArgument::CommandWithInputArgument(const std::optional<Argument>& argument) {
    if (!argument) return;

    if (argument.value().quoted) {
        m_in = std::make_unique<std::istringstream>(argument.value().value);
        return;
    }

    if (!std::filesystem::exists(argument.value().value)) {
        throw std::runtime_error("File does not exist: " + argument.value().value);
    }

    auto file = std::make_unique<std::ifstream>(argument.value().value);
    if (!*file) {
        throw std::runtime_error("Error opening a file: " + argument.value().value);
    }
    m_in = std::move(file);
}

void CommandWithInputArgument::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    if (m_in) do_execute(*m_in, out, err);
    else {
        do_execute(in, out, err);
        clearerr(stdin);
        std::cin.clear();
    }
}