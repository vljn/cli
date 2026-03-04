#include <chrono>

#include "Commands/Date.h"

CommandResult Date::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    const auto now = std::chrono::system_clock::now();
    auto local = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(now));
    out << std::format("{:%F}", local);

    return {};
}
