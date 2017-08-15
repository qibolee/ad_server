#include <algorithm>
#include "string_util.h"

std::string string_util::trim(const std::string &str) {
    auto iter1 = std::find_if_not(str.begin(), str.end(), isblank);
    auto iter2 = std::find_if_not(str.rbegin(), str.rend(), isblank);
    return std::string(iter1, iter2.base());
}

std::vector<std::string> string_util::split(const std::string &line, const std::string &delim) {

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

