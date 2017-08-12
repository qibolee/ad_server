#include <iostream>
#include <cstdio>
#include "json.hpp"
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>

using json = nlohmann::json;

int main() {
    
    json j;
    j["pi"] = 3.14;
    j["name"] = "¿Ó∆Ù≤®";
    j["age"] = 25;
    j["app"] = {"hello", "music", 23};
    j["dict"] = { {"hi", 2.324}, {"hey", "hey"} };
    std::cout << j.dump(4) << std::endl;
    printf("%s\n", j.dump().c_str());

    std::shared_mutex mutex_;


    return 0;
}

