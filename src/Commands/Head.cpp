#include "Commands/Head.h"

CommandResult Head::do_execute(std::istream& in, std::ostream& out, std::ostream& err) {
    std::string buffer;
    std::string toPrint;
    int i = 0;
    while (i != m_lineCount && std::getline(in, buffer)) {
        toPrint += buffer + '\n';
        i++;
    }
    out << toPrint;

    return {};
}