#include <algorithm>
#include <thread>
#include <cstring>
#include <cstdarg>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mutex>
#include "mlog.h"

std::size_t mlog::buf_size = 102400;
std::size_t mlog::tm_size = 64;

std::shared_ptr<char> mlog::debug_tm(new char[tm_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::trace_tm(new char[tm_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::warning_tm(new char[tm_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::fatal_tm(new char[tm_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::notice_tm(new char[tm_size], std::default_delete<char[]>());

int mlog::debug_fd = -1;
int mlog::trace_fd = -1;
int mlog::warning_fd = -1;
int mlog::fatal_fd = -1;
int mlog::notice_fd = -1;

std::shared_ptr<char> mlog::debug_buf(new char[buf_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::trace_buf(new char[buf_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::warning_buf(new char[buf_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::fatal_buf(new char[buf_size], std::default_delete<char[]>());
std::shared_ptr<char> mlog::notice_buf(new char[buf_size], std::default_delete<char[]>());

// mutex
std::mutex mlog::debug_mtx;
std::mutex mlog::trace_mtx;
std::mutex mlog::warning_mtx;
std::mutex mlog::fatal_mtx;
std::mutex mlog::notice_mtx;

void mlog::set_debug_path(const char *path) {
    if (path == NULL) {
        return;
    }
    std::lock_guard<std::mutex> lk(debug_mtx);
    if (debug_fd >= 0) {
        close(debug_fd);
    }
    debug_fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
}

void mlog::set_trace_path(const char *path) {
    if (path == NULL) {
        return;
    }
    std::lock_guard<std::mutex> lk(trace_mtx);
    if (trace_fd >= 0) {
        close(trace_fd);
    }
    trace_fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
}

void mlog::set_warning_path(const char *path) {
    if (path == NULL) {
        return;
    }
    std::lock_guard<std::mutex> lk(warning_mtx);
    if (warning_fd >= 0) {
        close(warning_fd);
    }
    warning_fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
}

void mlog::set_fatal_path(const char *path) {
    if (path == NULL) {
        return;
    }
    std::lock_guard<std::mutex> lk(fatal_mtx);
    if (fatal_fd >= 0) {
        close(fatal_fd);
    }
    fatal_fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
}

void mlog::set_notice_path(const char *path) {
    if (path == NULL) {
        return;
    }
    std::lock_guard<std::mutex> lk(notice_mtx);
    if (notice_fd >= 0) {
        close(notice_fd);
    }
    notice_fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
}

std::size_t mlog::get_time(char *buf, std::size_t len) {
    std::time_t t = std::time(NULL);
    return std::strftime(buf, len, "%Y/%m/%d %H:%M:%S", std::localtime(&t));
}

void mlog::notice(const notice_data &data) {
    std::lock_guard<std::mutex> lk(notice_mtx);
    if (notice_fd < 0) {
        return;
    }
    // ip, query, winfoid, show, charge, title, desc1, desc2, showurl, targeturl
    int len = snprintf(notice_buf.get(), buf_size, "%s\t%s\t%lu\t%d\t%d\t%s\t%s\t%s\t%s\t%s\n", data.ip.c_str(), data.query.c_str(), data.winfoid, data.show, data.charge, data.title.c_str(), data.desc1.c_str(), data.desc2.c_str(), data.showurl.c_str(), data.targeturl.c_str());
    write(notice_fd, notice_buf.get(), len);
}

void mlog::log(int level, const char *func, const char *file, int line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    switch (level) {
        case MDEBUG:
            debug(func, file, line, fmt, args);
            break;
        case MTRACE:
            trace(func, file, line, fmt, args);
            break;
        case MWARNING:
            warning(func, file, line, fmt, args);
            break;
        case MFATAL:
            fatal(func, file, line, fmt, args);
            break;
    }
    va_end(args);
}

void mlog::debug(const char *func, const char *file, int line, const char *fmt, va_list args) {
    std::lock_guard<std::mutex> lk(debug_mtx);
    if (debug_fd < 0) {
        return;
    }
    get_time(debug_tm.get(), tm_size);
    int len = snprintf(debug_buf.get(), buf_size, "[%s DEBUG %llu %s@%s: %d] ", debug_tm.get(), std::this_thread::get_id(), func, file, line);
    int len2 = vsnprintf(debug_buf.get() + len, buf_size - len, fmt, args);
    //snprintf(debug_buf + len + len2, buf_size - len - len2, "\n");
    write(debug_fd, debug_buf.get(), len + len2);
}

void mlog::trace(const char *func, const char *file, int line, const char *fmt, va_list args) {
    std::lock_guard<std::mutex> lk(trace_mtx);
    if (trace_fd < 0) {
        return;
    }
    get_time(trace_tm.get(), tm_size);
    int len = snprintf(trace_buf.get(), buf_size, "[%s TRACE %llu %s@%s: %d] ", trace_tm.get(), std::this_thread::get_id(), func, file, line);
    int len2 = vsnprintf(trace_buf.get() + len, buf_size - len, fmt, args);
    write(trace_fd, trace_buf.get(), len + len2);
}

void mlog::warning(const char *func, const char *file, int line, const char *fmt, va_list args) {
    std::lock_guard<std::mutex> lk(warning_mtx);
    if (warning_fd < 0) {
        return;
    }
    get_time(warning_tm.get(), tm_size);
    int len = snprintf(warning_buf.get(), buf_size, "[%s WARNING %llu %s@%s: %d] ", warning_tm.get(), std::this_thread::get_id(), func, file, line);
    int len2 = vsnprintf(warning_buf.get() + len, buf_size - len, fmt, args);
    write(warning_fd, warning_buf.get(), len + len2);
}

void mlog::fatal(const char *func, const char *file, int line, const char *fmt, va_list args) {
    std::lock_guard<std::mutex> lk(fatal_mtx);
    if (fatal_fd < 0) {
        return;
    }
    get_time(fatal_tm.get(), tm_size);
    int len = snprintf(fatal_buf.get(), buf_size, "[%s FATAL %llu %s@%s: %d] ", fatal_tm.get(), std::this_thread::get_id(), func, file, line);
    int len2 = vsnprintf(fatal_buf.get() + len, buf_size - len, fmt, args);
    write(fatal_fd, fatal_buf.get(), len + len2);
}




