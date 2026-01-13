#include <fstream>
#include <filesystem>
#include "Touch.h"

void Touch::execute(std::istream &in, std::ostream &out) {
    if (std::filesystem::exists(m_filename)) throw std::runtime_error("File already exists: " + m_filename);
    std::ofstream file(m_filename);
    if (!file.is_open()) throw std::runtime_error("Error creating a file: " + m_filename);
}
