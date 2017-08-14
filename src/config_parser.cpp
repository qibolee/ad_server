#include <algorithm>
#include <vector>
#include "config_parser.h"



config_parser::config_parser(): istrm(), secPtn("^\\[[\\S]+\\]"), kvPtn("[\\S]+\\s*=\\s*[\\S]+") {

}

config_parser::config_parser(const char *filename): istrm(filename, std::ios::binary), secPtn("^\\[[\\S]+\\]"), kvPtn("[\\S]+\\s*=\\s*[\\S]+") {

}

config_parser::~config_parser() {
    close();
}

bool config_parser::open(const char *filename) {
    close();
    istrm.open(filename, std::ios::binary);
    return istrm.is_open();
}

void config_parser::close() {
    if (istrm.is_open()) {
        istrm.close();
    }
}

std::string config_parser::trim(const std::string &str) {
    auto iter1 = find_if_not(str.begin(), str.end(), isblank);
    auto iter2 = find_if_not(str.rbegin(), str.rend(), isblank);
    return std::string(iter1, iter2.base());
}

std::vector<std::string> config_parser::split(const std::string line, const std::string delim) {
    std::vector<std::string> ret;
    std::size_t last = 0;
    std::size_t idx = line.find_first_of(delim, last);
    while (idx != std::string::npos) {
        ret.push_back(line.substr(last, idx - last));
        last = idx + 1;
        idx = line.find_first_of(delim, last);
    }
    if (last < line.size()) {
        ret.push_back(line.substr(last));
    }
    return ret;
}

std::string config_parser::get(const std::string &section, const std::string &key) {
    std::string line;
    std::string target = "[" + section + "]";
    bool found_section = false;
    istrm.seekg(0);
    while (getline(istrm, line)) {
        line = trim(line);
        if (found_section) {
            // find key
            if (!std::regex_match(line, kvPtn)) {
                break;
            }
            std::vector<std::string> kvList = split(line, "=");
            if (trim(kvList[0]) == key) {
                return trim(kvList[1]);
            }
        } else {
            // find section
            if (!std::regex_match(line, secPtn)) {
                continue;
            }
            if (target == trim(line)) {
                // found
                found_section = true;
            }
        }
    }

    return "";
}




