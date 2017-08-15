#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <regex>


class config_parser {
public:
    config_parser();
    config_parser(const char *filename);
    ~config_parser();


    // open file stream
    bool open(const char *filename);

    // close file
    void close();
    
    // get a value according to section and key
    std::string get(const std::string &section, const std::string &key);

private:
    config_parser(const config_parser &) = delete;
    config_parser(config_parser &&) = delete;
    config_parser &operator=(const config_parser &) = delete;
    config_parser &operator=(config_parser &&) = delete;

    // ifstream
    std::ifstream istrm;

    // regex to match string
    std::regex secPtn;
    std::regex kvPtn;
};



#endif //CONFIG_PARSER_H
