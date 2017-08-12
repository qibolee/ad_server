#ifndef RELOAD_THREAD_H
#define RELOAD_THREAD_H


class reload_thread {
public:
    reload_thread() = default;
    void operator()();
/*
    reload_thread(const reload_thread &) = delete;
    reload_thread(reload_thread &&) = delete;
    reload_thread &operator=(const reload_thread &) = delete;
    reload_thread &operator=(reload_thread &&) = delete;
*/
private:
    void do_reload();
};


#endif //RELOAD_THREAD_H


