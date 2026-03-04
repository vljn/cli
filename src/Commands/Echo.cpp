#include <string>

#include "Commands/Echo.h"

CommandResult Echo::do_execute(std::istream& in, std::ostream& out, std::ostream& err) {
    std::string line;
    std::string toPrint;
    bool first = true;

    while (std::getline(in, line)) {
        if (!first) {
            toPrint += "\n";
        }
        toPrint += line;
        first = false;
    }

    out << toPrint;
    return {};
}