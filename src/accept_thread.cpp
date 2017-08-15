#include <sys/types.h>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include "accept_thread.h"
#include "mlog.h"
#include "inet_sockets.h"
#include "config.h"


extern std::shared_ptr<config> cfg;

const int accept_thread::backlog = 1024;
const std::size_t accept_thread::addrStrLen = 4096;

accept_thread::accept_thread(const std::shared_ptr<blocking_queue<int>> &q): sfd(-1), queue(q), cfd(-1), addrStr(new char[addrStrLen], std::default_delete<char[]>()) {
    MLOG(MDEBUG, "accept thread init done");
    if (q.get() == NULL) {
        MLOG(MFATAL, "blocking queue is null");
    }
}

accept_thread::~accept_thread() {
    if (sfd >= 0) {
        close(sfd);
    }
}

void accept_thread::operator()() {
    MLOG(MDEBUG, "accept thread started");

    if (queue.get() == NULL) {
        MLOG(MFATAL, "blocking queue is null");
        return;
    }

    if (!init()) {
        MLOG(MFATAL, "accept thread init error");
        return;
    }
    MLOG(MDEBUG, "socket listening");
    while (true) {
        clear();

        cfd = accept(sfd, (struct sockaddr *)&addr, &addrlen);
        if (cfd == -1) {
            MLOG(MWARNING, "accept error");
            continue;
        }
        MLOG(MDEBUG, "accept client socket fd: %d", cfd);

        inet_sockets::inet_addr_str((struct sockaddr *)&addr, addrlen, addrStr.get(), addrStrLen);
        MLOG(MDEBUG, "accept socket %d from client %s", cfd, addrStr.get());

        queue->put(cfd);
        MLOG(MDEBUG, "put fd[%d] to blocking queue", cfd);
    }
}

bool accept_thread::init() {
    sfd = inet_sockets::inet_listen(cfg->get_service_port().c_str(), backlog, (struct sockaddr *)&addr, &addrlen);
    if (sfd == -1) {
        MLOG(MFATAL, "listen socket create error");
        return false;
    }
    inet_sockets::inet_addr_str((struct sockaddr *)&addr, addrlen, addrStr.get(), addrStrLen);
    MLOG(MDEBUG, "create listening socket fd: %d, address: %s", sfd, addrStr.get());
    return true;
}

void accept_thread::clear() {
    cfd = -1;
    std::fill_n(addrStr.get(), addrStrLen, '\0');
    std::snprintf(addrStr.get(), addrStrLen, "[UNKNOW]");
}



