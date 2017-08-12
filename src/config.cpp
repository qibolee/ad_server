#include <mutex>
#include <shared_mutex>
#include <string>
#include "config.h"



std::string config::get_debug_log_path() {
    std::shared_lock<std::shared_timed_mutex> lk(debugLogPathMtx);
    return debugLogPath;
}

bool config::set_debug_log_path(const std::string &path) {
    std::unique_lock<std::shared_timed_mutex> lk(debugLogPathMtx);
    debugLogPath = path;
    return true;
}

std::string config::get_trace_log_path() {
    std::shared_lock<std::shared_timed_mutex> lk(traceLogPathMtx);
    return traceLogPath;
}

bool config::set_trace_log_path(const std::string &path) {
    std::unique_lock<std::shared_timed_mutex> lk(traceLogPathMtx);
    traceLogPath = path;
    return true;
}

std::string config::get_warning_log_path() {
    std::shared_lock<std::shared_timed_mutex> lk(warningLogPathMtx);
    return warningLogPath;
}

bool config::set_warning_log_path(const std::string &path) {
    std::unique_lock<std::shared_timed_mutex> lk(warningLogPathMtx);
    warningLogPath = path;
    return true;
}

std::string config::get_fatal_log_path() {
    std::shared_lock<std::shared_timed_mutex> lk(fatalLogPathMtx);
    return fatalLogPath;
}

bool config::set_fatal_log_path(const std::string &path) {
    std::unique_lock<std::shared_timed_mutex> lk(fatalLogPathMtx);
    fatalLogPath = path;
    return true;
}

std::string config::get_notice_log_path() {
    std::shared_lock<std::shared_timed_mutex> lk(noticeLogPathMtx);
    return noticeLogPath;
}

bool config::set_notice_log_path(const std::string &path) {
    std::unique_lock<std::shared_timed_mutex> lk(noticeLogPathMtx);
    noticeLogPath = path;
    return true;
}

std::string config::get_service_port() {
    std::shared_lock<std::shared_timed_mutex> lk(servicePortMtx);
    return servicePort;
}

bool config::set_service_port(const std::string &port) {
    std::unique_lock<std::shared_timed_mutex> lk(servicePortMtx);
    servicePort = port;
    return true;
}

size_t config::get_max_adnum() {
    std::shared_lock<std::shared_timed_mutex> lk(maxAdNumMtx);
    return maxAdNum;
}

bool config::set_max_adnum(size_t num) {
    std::unique_lock<std::shared_timed_mutex> lk(maxAdNumMtx);
    maxAdNum = num;
    return true;
}

size_t config::get_max_bidword_num() {
    std::shared_lock<std::shared_timed_mutex> lk(maxBidwordNumMtx);
    return maxBidwordNum;
}

bool config::set_max_bidword_len(size_t len) {
    std::unique_lock<std::shared_timed_mutex> lk(maxBidwordNumMtx);
    maxBidwordLen = len;
    return true;
}

size_t config::get_max_bidword_len() {
    std::shared_lock<std::shared_timed_mutex> lk(maxBidwordLenMtx);
    return maxBidwordLen;
}

bool config::set_max_bidword_num(size_t num) {
    std::unique_lock<std::shared_timed_mutex> lk(maxBidwordNumMtx);
    maxBidwordNum = num;
    return true;
}

std::string config::get_ad_file() {
    std::shared_lock<std::shared_timed_mutex> lk(adFileMtx);
    return adFile;
}

bool config::set_ad_file(const std::string &path) {
    std::unique_lock<std::shared_timed_mutex> lk(adFileMtx);
    adFile = path;
    return true;
}

int config::get_reload_interval() {
    std::shared_lock<std::shared_timed_mutex> lk(reloadIntervalMtx);
    return reloadInterval;
}

bool config::set_reload_interval(int i) {
    std::unique_lock<std::shared_timed_mutex> lk(reloadIntervalMtx);
    reloadInterval = i;
    return true;
}

int config::get_max_process_num() {
    std::shared_lock<std::shared_timed_mutex> lk(maxProcessNumMtx);
    return maxProcessNum;
}

bool config::set_max_process_num(int i) {
    std::unique_lock<std::shared_timed_mutex> lk(maxProcessNumMtx);
    maxProcessNum = i;
    return true;
}



