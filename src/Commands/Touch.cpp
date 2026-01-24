#include <fstream>
#include <filesystem>

#include "Touch.h"

Command::CommandResult Touch::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    if (std::filesystem::exists(m_filename)) {
        err << "File already exists: " << m_filename << std::endl;
        return {};
    }
    std::ofstream file(m_filename);
    if (!file.is_open()) {
        err << "Error creating a file: " << m_filename << std::endl;
        return {};
    }
    return {};
}
