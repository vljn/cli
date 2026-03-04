#include "Commands/Head.h"

CommandResult Head::do_execute(std::istream& in, std::ostream& out, std::ostream& err) {
    std::string buffer;
    std::string toPrint;
    int i = 0;
    bool first = true;
    while (std::getline(in, buffer)) {
        if (m_lineCount > i++) {
            if (!first) {
                toPrint += "\n";
            }
            else {
                first = false;
            }
            toPrint += buffer;
        }
        if (&in != &std::cin) {
            break;
        }
    }
    out << toPrint;
    out.flush();

    return {};
}