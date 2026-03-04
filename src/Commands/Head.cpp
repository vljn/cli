#include "Commands/Head.h"

CommandResult Head::do_execute(std::istream& in, std::ostream& out, std::ostream& err) {
    std::string buffer;
    std::string toPrint;
    int i = 0;
    while (std::getline(in, buffer)) {
        if (m_lineCount > i++) {
            toPrint += buffer + '\n';
        }
        if (&in != &std::cin) {
            break;
        }
    }
    out << toPrint;

    return {};
}