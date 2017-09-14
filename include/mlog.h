#ifndef MLOG_H
#define MLOG_H

#include <mutex>
#include <memory>
#include <ctime>
#include <cstdarg>
#include <iomanip>
#include "notice_data.h"


enum {
    MDEBUG,
    MTRACE,
    MWARNING,
    MFATAL
};


//#define MLOG(level, format, ...) fprintf(stderr, "[%s %s %s@%s: %d] " format "\n", level, currTime("%Y/%m/%d %H:%M:%S"), __func__, __FILE__, __LINE__, ##__VA_ARGS__)
#define MLOG(level, format, ...) mlog::log(level, __func__, __FILE__, __LINE__, format"\n", ##__VA_ARGS__)

//void mlog(LogLevel level, const char *file, const char *func, const int line, const char *format, ...);

class mlog {
public:
    static void init(const char *debug_path, const char *trace_path, const char *warning_path, const char *fatal_path, const char *notice_path);
    static void log(int level, const char *func, const char *file, int line, const char *fmt, ...);
    static void notice(const notice_data &data);

    static void set_debug_path(const char *path);
    static void set_trace_path(const char *path);
    static void set_warning_path(const char *path);
    static void set_fatal_path(const char *path);
    static void set_notice_path(const char *path);

private:
    static void debug(const char *func, const char *file, int line, const char *fmt, va_list args);
    static void trace(const char *func, const char *file, int line, const char *fmt, va_list args);
    static void warning(const char *func, const char *file, int line, const char *fmt, va_list args);
    static void fatal(const char *func, const char *file, int line, const char *fmt, va_list args);

    static std::size_t get_time(char *buf, std::size_t len);
    // fd
    static int debug_fd;
    static int trace_fd;
    static int warning_fd;
    static int fatal_fd;
    static int notice_fd;
    // log info buf
    static std::size_t buf_size;
    static std::shared_ptr<char> debug_buf;
    static std::shared_ptr<char> trace_buf;
    static std::shared_ptr<char> warning_buf;
    static std::shared_ptr<char> fatal_buf;
    static std::shared_ptr<char> notice_buf;
    // time info buf
    static std::size_t tm_size;
    static std::shared_ptr<char> debug_tm;
    static std::shared_ptr<char> trace_tm;
    static std::shared_ptr<char> warning_tm;
    static std::shared_ptr<char> fatal_tm;
    static std::shared_ptr<char> notice_tm;
    // mutex
    static std::mutex debug_mtx;
    static std::mutex trace_mtx;
    static std::mutex warning_mtx;
    static std::mutex fatal_mtx;
    static std::mutex notice_mtx;
};



#endif //MLOG_H




