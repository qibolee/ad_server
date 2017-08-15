#include <iostream>
#include <string>
#include <cstdio>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    
    json j;
    j["pi"] = 3.14;
    j["name"] = "������";
    j["age"] = 25;
    j["app"] = {"hello", "music", 23};
    j["dict"] = { {"hi", 2.324}, {"hey", "hey"} };
    std::cout << j.dump(4) << std::endl;
    printf("%s\n", j.dump().c_str());

    std::string str = "{\"bidwords\": [\"�ʻ�\"], \"adnum\": 2}";
    nlohmann::json j2 = nlohmann::json::parse(str);



    return 0;
}

