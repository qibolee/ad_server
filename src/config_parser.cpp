#include "config_parser.h"
#include "string_util.h"



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

std::string config_parser::get(const std::string &section, const std::string &key) {
    std::string line;
    std::string target = "[" + section + "]";
    bool found_section = false;
    istrm.seekg(0);
    while (getline(istrm, line)) {
        line = string_util::trim(line);
        if (found_section) {
            // find key
            if (!std::regex_match(line, kvPtn)) {
                break;
            }
            std::vector<std::string> kvList = string_util::split(line, "=");
            if (string_util::trim(kvList[0]) == key) {
                return string_util::trim(kvList[1]);
            }
        } else {
            // find section
            if (!std::regex_match(line, secPtn)) {
                continue;
            }
            if (target == string_util::trim(line)) {
                // found
                found_section = true;
            }
        }
    }

    return "";
}




