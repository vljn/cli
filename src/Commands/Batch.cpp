#include <filesystem>
#include <fstream>

#include "Batch.h"

Command::CommandResult Batch::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    if (!std::filesystem::exists(m_filename)) {
        err << "File does not exist: " << m_filename << std::endl;
        return {};
    }
    auto file = new std::ifstream(m_filename);
    if (!file->is_open()) {
        err << "Error opening a file: " << m_filename << std::endl;
        return {};
    }
    return {InterpreterAction::PushStream, file};
}
