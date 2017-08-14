#ifndef ACCEPT_THREAD_H
#define ACCEPT_THREAD_H

#include <memory>
#include <netinet/in.h>
#include "blocking_queue.h"

class accept_thread {
public:
    accept_thread(const std::shared_ptr<blocking_queue<int>> &q);
    ~accept_thread();

    void operator()();
/*
    accept_thread(const accept_thread &) = delete;
    accept_thread(accept_thread &&) = delete;
    accept_thread &operator=(const accept_thread &) = delete;
    accept_thread &operator=(accept_thread &&) = delete;
*/    
private:
    // init service to listening
    bool init();

    // clear thread data, prepare for next process
    void clear();

    // service socket fd
    int sfd;
    
    // blcoking queue to save socket fd;
    std::shared_ptr<blocking_queue<int>> queue;

    // local listening service info
    static const int backlog;

    // socket client info
    int cfd;
    sockaddr_storage addr;
    socklen_t addrlen;
    std::shared_ptr<char> addrStr;
    static const std::size_t addrStrLen;
};



#endif //ACCEPT_THREAD_H


