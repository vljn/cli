#include "Commands/Rm.h"

#include <filesystem>

CommandResult Rm::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    if (!std::filesystem::exists(m_filename)) {
        err << "file does not exist: " << m_filename << std::endl;
        return {};
    }

    try {
        std::filesystem::remove(m_filename);
    }
    catch (const std::filesystem::filesystem_error& e) {
        err << "error removing file: " << e.what() << std::endl;
    }
    return {};
}
