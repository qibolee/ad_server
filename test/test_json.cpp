#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <iconv.h>
#include "json.hpp"

using json = nlohmann::json;

int encoding_convert(const char *from, const char *to, char *inbuf, std::size_t inlen, char *outbuf, std::size_t outlen) {
    iconv_t cod = iconv_open(from, to);
    if (cod == 0) {
        std::cerr << "iconv open error" << std::endl;
        return -1;
    }
    std::memset(outbuf, 0, outlen);
    if (iconv(cod, &inbuf, &inlen, &outbuf, &outlen) == -1) {
        std::cerr << "iconv error" << std::endl;
        iconv_close(cod);
        return -1;
    }
    iconv_close(cod);
    return 0;
}

int main() {
    
    json j;
    j["pi"] = 3.14;
    j["name"] = "ÀîÆô²¨";
    j["age"] = 25;
    j["app"] = {"hello", "music", 23};
    j["dict"] = { {"hi", 2.324}, {"hey", "hey"} };
    std::cout << j.dump(4) << std::endl;
    json jj = {
        {"pi", 3.14},
        {"name", "ÀîÆô²¨"},
        {"app", {"hello", 234}}
    };
    std::cout << jj.dump(4) << std::endl;

    std::size_t inlen = 256;
    char *inbuf = new char[inlen];
    std::snprintf(inbuf, inlen, "{\"bidwords\": [\"flower\"], \"adnum\": 2}");
    std::size_t outlen = 256;
    char *outbuf = new char[outlen];
    std::printf("%s\n", inbuf);
    if (encoding_convert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen) != 0) {
        std::cerr << "iconv failed" << std::endl;
    } else {
        std::printf("%s\n", outbuf);
    }

    nlohmann::json j2 = nlohmann::json::parse(inbuf);

    delete [] inbuf;
    delete [] outbuf;

    return 0;
}
