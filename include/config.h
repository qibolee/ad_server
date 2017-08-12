#ifndef CONFIG_H
#define CONFIG_H

#include <shared_mutex>
#include <string>


class config {
public:
    config() = default;

    std::string get_debug_log_path();
    bool set_debug_log_path(const std::string &path);

    std::string get_trace_log_path();
    bool set_trace_log_path(const std::string &path);

    std::string get_warning_log_path();
    bool set_warning_log_path(const std::string &path);

    std::string get_fatal_log_path();
    bool set_fatal_log_path(const std::string &path);

    std::string get_notice_log_path();
    bool set_notice_log_path(const std::string &path);

    std::string get_service_port();
    bool set_service_port(const std::string &port);

    size_t get_max_adnum();
    bool set_max_adnum(size_t num);

    size_t get_max_bidword_num();
    bool set_max_bidword_num(size_t num);

    size_t get_max_bidword_len();
    bool set_max_bidword_len(size_t len);

    std::string get_ad_file();
    bool set_ad_file(const std::string &path);

    int get_reload_interval();
    bool set_reload_interval(int i);

    int get_max_process_num();
    bool set_max_process_num(int i);
    
private:
    config(const config &) = delete;
    config(config &&) = delete;
    config &operator=(const config &) = delete;
    config &operator=(config &&) = delete;
    
    // log path info
    std::string debugLogPath;
    mutable std::shared_timed_mutex debugLogPathMtx;
    std::string traceLogPath;
    mutable std::shared_timed_mutex traceLogPathMtx;
    std::string warningLogPath;
    mutable std::shared_timed_mutex warningLogPathMtx;
    std::string fatalLogPath;
    mutable std::shared_timed_mutex fatalLogPathMtx;
    std::string noticeLogPath;
    mutable std::shared_timed_mutex noticeLogPathMtx;

    // service listen port info
    std::string servicePort;
    mutable std::shared_timed_mutex servicePortMtx;

    // max ad num info
    size_t maxAdNum;
    mutable std::shared_timed_mutex maxAdNumMtx;

    // max bidword num info
    size_t maxBidwordNum;
    mutable std::shared_timed_mutex maxBidwordNumMtx;

    // max bidword len info
    size_t maxBidwordLen;
    mutable std::shared_timed_mutex maxBidwordLenMtx;

    // ad file info
    std::string adFile;
    mutable std::shared_timed_mutex adFileMtx;

    // reload interval info
    int reloadInterval;
    mutable std::shared_timed_mutex reloadIntervalMtx;

    // max thread num
    int maxProcessNum;
    mutable std::shared_timed_mutex maxProcessNumMtx;
};


#endif //CONFIG_H

