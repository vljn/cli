#include "Commands/Tr.h"

CommandResult Tr::do_execute(std::istream& in, std::ostream& out, std::ostream& err) {
    char ch;
    int index = 0;
    int indexMatch = 0;
    int indexWhat = 0;
    std::string toPrint;
    while (in.get(ch)) {
        toPrint += ch;
        if (indexWhat == m_what.size()) {
            indexWhat = 0;
            toPrint.replace(indexMatch, m_what.size(), m_with);
            index = indexMatch + m_with.size();
        }
        if (ch == m_what[indexWhat]) {
            if (indexWhat == 0) {
                indexMatch = index;
            }
            indexWhat++;
        }
        else {
            indexWhat = 0;

        }
        index++;
    }
    out << toPrint << std::endl;

    return {};
}
