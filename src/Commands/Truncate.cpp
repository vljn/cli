#include <filesystem>
#include <fstream>

#include "Commands/Truncate.h"


CommandResult Truncate::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    if (!std::filesystem::exists(m_filename)) {
        err << "file does not exist: " << m_filename << std::endl;
        return {};
    }

    std::ofstream file(m_filename, std::ios::trunc);

    if (!file.is_open()) {
        err << "error opening a file: " << m_filename << std::endl;
        return {};
    }

    file.close();

    return {};
}
