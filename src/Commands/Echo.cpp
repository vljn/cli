#include <string>

#include "Echo.h"

void Echo::do_execute(std::istream &in, std::ostream &out, std::ostream& err) {
    std::string toPrint;
    std::string buffer;
    while (std::getline(in, buffer)) {
        toPrint += buffer + '\n';
    }
    out << toPrint;
}