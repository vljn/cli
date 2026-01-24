#include <chrono>

#include "Commands/Time.h"

Command::CommandResult Time::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    const auto now = std::chrono::system_clock::now();
    const auto local = std::chrono::zoned_time{"Europe/Belgrade", now};
    out << std::format("{:%H:%M}", local) << std::endl;

    return {};
}
