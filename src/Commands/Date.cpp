#include <chrono>

#include "Date.h"

Command::CommandResult Date::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    auto now = std::chrono::system_clock::now();
    auto local = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(now));
    out << std::format("{:%F}", local) << std::endl;

    return {};
}
