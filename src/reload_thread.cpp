#include <thread>
#include <chrono>
#include <memory>
#include "reload_thread.h"
#include "config.h"
#include "config_parser.h"


extern std::shared_ptr<config> cfg;
extern std::shared_ptr<config_parser> parser;

void reload_thread::operator()() {
    while (true) {
        do_reload();
        std::this_thread::sleep_for(std::chrono::seconds(cfg->get_reload_interval()));
    }
}

void reload_thread::do_reload() {


}




