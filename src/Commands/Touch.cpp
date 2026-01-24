#include <fstream>
#include <filesystem>

#include "Commands/Touch.h"

Command::CommandResult Touch::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    if (std::filesystem::exists(m_filename)) {
        err << "file already exists: " << m_filename << std::endl;
        return {};
    }

    std::ofstream file(m_filename);

    if (!file.is_open()) {
        err << "error creating a file: " << m_filename << std::endl;
        return {};
    }

    file.close();

    return {};
}
