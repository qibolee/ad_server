#include <chrono>
#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <vector>
#include <algorithm>
#include <memory>
#include "mlog.h"
#include "config.h"
#include "config_parser.h"
#include "blocking_queue.h"
#include "accept_thread.h"
#include "process_thread.h"
#include "reload_thread.h"



std::shared_ptr<config> cfg;
std::shared_ptr<config_parser>parser;
const char *main_config_path = "./config/main.cfg";

void init_service();
void start_service();

int main(int argc, const char **argv) {

    init_service();

    start_service();

    return 0;
}

void init_service() {
    cfg = std::make_shared<config>();
    parser = std::make_shared<config_parser>(main_config_path);
    // log path
    cfg->set_debug_log_path(parser->get("log", "debug_path"));
    //std::cerr << "#" << cfg->get_debug_log_path() << "#" << std::endl;
    mlog::set_debug_path(cfg->get_debug_log_path().c_str());
    MLOG(MDEBUG, "open debug log");

    cfg->set_trace_log_path(parser->get("log", "trace_path"));
    //std::cerr << "#" << cfg->get_trace_log_path() << "#" << std::endl;
    mlog::set_trace_path(cfg->get_trace_log_path().c_str());
    MLOG(MTRACE, "open trace log");

    cfg->set_warning_log_path(parser->get("log", "warning_path"));
    //std::cerr << "#" << cfg->get_warning_log_path() << "#" << std::endl;
    mlog::set_warning_path(cfg->get_warning_log_path().c_str());
    MLOG(MWARNING, "open warning log");

    cfg->set_fatal_log_path(parser->get("log", "fatal_path"));
    //std::cerr << "#" << cfg->get_fatal_log_path() << "#" << std::endl;
    mlog::set_fatal_path(cfg->get_fatal_log_path().c_str());
    MLOG(MFATAL, "open fatal log");

    cfg->set_notice_log_path(parser->get("log", "notice_path"));
    //std::cerr << "#" << cfg->get_notice_log_path() << "#" << std::endl;
    mlog::set_notice_path(cfg->get_notice_log_path().c_str());
    
    // service
    std::string service_port = parser->get("service", "port");
    MLOG(MDEBUG, "config: service port: %s", service_port.c_str());
    cfg->set_service_port(service_port);
    
    // reload
    std::string reload_interval = parser->get("reload", "interval");
    MLOG(MDEBUG, "config: reload interval: %s", reload_interval.c_str());
    cfg->set_reload_interval(std::stoi(reload_interval));
    
    // ad
    std::string ad_max_adnum = parser->get("ad", "max_adnum");
    MLOG(MDEBUG, "config: ad max_adnum: %s", ad_max_adnum.c_str());
    cfg->set_max_adnum(std::stoull(ad_max_adnum));

    std::string ad_max_bidword_num = parser->get("ad", "max_bidword_num");
    MLOG(MDEBUG, "config: ad max_bidword_num: %s", ad_max_bidword_num.c_str());
    cfg->set_max_bidword_num(std::stoull(ad_max_bidword_num));
    
    std::string ad_max_bidword_len = parser->get("ad", "max_bidword_len");
    MLOG(MDEBUG, "config: ad max_bidword_len: %s", ad_max_bidword_len.c_str());
    cfg->set_max_bidword_len(std::stoull(ad_max_bidword_len));
    
    std::string ad_ad_file = parser->get("ad", "ad_file");
    MLOG(MDEBUG, "config: ad ad_file: %s", ad_ad_file.c_str());
    cfg->set_ad_file(ad_ad_file);
    
    // max thread num
    std::string thread_max_process = parser->get("thread", "max_process");
    MLOG(MDEBUG, "config: thread max_process: %s", thread_max_process.c_str());
    cfg->set_max_process_num(std::stoi(thread_max_process));

}

void start_service() {
    std::shared_ptr<blocking_queue<int>> queue(new blocking_queue<int>(1024));

//    accept_thread aThreadObj(queue);
//    process_thread pThreadObj(queue);
//    reload_thread rThreadObj;

    std::thread acceptThread{accept_thread(queue)};
    std::thread reloadThread{reload_thread()};
    std::vector<std::thread> processThreads;
    for (auto i = 0; i < cfg->get_max_process_num(); ++i) {
        processThreads.emplace(processThreads.end(), process_thread(queue));
    }

    acceptThread.join();
    reloadThread.join();
    std::for_each(processThreads.begin(), processThreads.end(), std::mem_fn(&std::thread::join));

}



