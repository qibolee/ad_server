#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <vector>
#include <string>


class string_util {
public:
    static std::string trim(const std::string &str);
    static std::vector<std::string> split(const std::string &line, const std::string &delim);
    static std::vector<std::string> split(const std::string &line, char delim);
};


#endif //STRING_UTIL_H
