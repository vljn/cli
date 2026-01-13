#include "Wc.h"

void Wc::do_execute(std::istream &in, std::ostream &out) {
    int count = 0;
    if (m_mode == Mode::Words) {
        std::string buffer;
        while (in >> buffer) {
            count++;
        }
    }
    else {
        char c;
        while (in.get(c)) count++;
    }
    out << count << std::endl;
}
