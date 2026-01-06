#include <chrono>
#include "../Core/Command.h"
#include "Time.h"

Time::Time() : Command(CommandInvocation{}) {}

void Time::execute(std::istream* in, std::ostream* out) {
    auto now = std::chrono::system_clock::now();
    auto local = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(now));
    *out << std::format("{:%T}", local) << std::endl;
}
