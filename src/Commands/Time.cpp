#include <chrono>

#include "Commands/Time.h"

CommandResult Time::execute(std::istream& in, std::ostream& out, std::ostream& err) {
    const auto now = std::chrono::system_clock::now();
    auto zone = std::chrono::current_zone();
    const auto local = std::chrono::zoned_time{zone, now};
    out << std::format("{:%H:%M}", local);

    return {};
}
