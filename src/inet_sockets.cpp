#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <cstdio>
#include "inet_sockets.h"

int inet_sockets::inet_connect(const char *host, const char *service, int type) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd = 0;

    if (host == NULL || service == NULL) {
        return -1;
    }

    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = type;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if (getaddrinfo(host, service, &hints, &result) != 0) {
        errno = ENOSYS; 
        return -1;
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }
        close(sfd);
    }
    freeaddrinfo(result);
    
    return rp == NULL ? -1 : sfd;
}

int inet_sockets::inet_listen(const char *service, int backlog, struct sockaddr *addr, socklen_t *addrlen) {
    if (service == NULL) {
        return -1;
    }
    return inet_passive_socket(service, SOCK_STREAM, true, backlog, addr, addrlen);
}

int inet_sockets::inet_bind(const char *service, int type, struct sockaddr *addr, socklen_t *addrlen) {
    if (service == NULL) {
        return -1;
    }
    return inet_passive_socket(service, type, false, 0, addr, addrlen);
}

int inet_sockets::get_sock_addr(int fd, char *addrStr, int addrStrLen) {
    if (fd < 0 || addrStr == NULL) {
        return -1;
    }
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    if (getsockname(fd, (struct sockaddr *)&addr, &addrlen) != 0) {
        return -1;
    }
    if (inet_addr_str((struct sockaddr *)&addr, addrlen, addrStr, addrStrLen) == -1) {
        return -1;
    }
    return 0;
}

int inet_sockets::get_sock_addr(int fd, struct sockaddr *addr, socklen_t *addrlen, char *addrStr, int addrStrLen) {
    if (fd < 0 || addr == NULL || addrlen == NULL || addrStr == NULL) {
        return -1;
    }
    if (getsockname(fd, addr, addrlen) != 0) {
        return -1;
    }
    if (inet_addr_str(addr, *addrlen, addrStr, addrStrLen) == -1) {
        return -1;
    }
    return 0;
}

int inet_sockets::get_peer_addr(int fd, char *addrStr, int addrStrLen) {
    if (fd < 0 || addrStr == NULL) {
        return -1;
    }
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    if (getpeername(fd, (struct sockaddr *)&addr, &addrlen) != 0) {
        return -1;
    }
    if (inet_addr_str((struct sockaddr *)&addr, addrlen, addrStr, addrStrLen) == -1) {
        return -1;
    }
    return 0;
}

int inet_sockets::get_peer_addr(int fd, struct sockaddr *addr, socklen_t *addrlen, char *addrStr, int addrStrLen) {
    if (fd < 0 || addr == NULL || addrlen == NULL || addrStr == NULL) {
        return -1;
    }
    if (getpeername(fd, addr, addrlen) != 0) {
        return -1;
    }
    if (inet_addr_str(addr, *addrlen, addrStr, addrStrLen) == -1) {
        return -1;
    }
    return 0;
}

int inet_sockets::inet_addr_str(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen) {
    if (addr == NULL || addrStr == NULL) {
        return -1;
    }
    char host[NI_MAXHOST], service[NI_MAXSERV];
    if (getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        std::snprintf(addrStr, addrStrLen, "[%s, %s]", host, service);
    } else {
        std::snprintf(addrStr, addrStrLen, "[?UNKNOW?]");
    }
    addrStr[addrStrLen - 1] = '\0';
    return 0;
}

int inet_sockets::inet_passive_socket(const char *service, int type, bool doListen, int backlog, struct sockaddr *addr, socklen_t *addrlen) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval;

    if (service == NULL) {
        return -1;
    }

    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = type;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_canonname = NULL;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, service, &hints, &result) != 0) {
        errno = ENOSYS;
        return -1;
    }

    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }
        if (doListen) {
            if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
                close(sfd);
                freeaddrinfo(result);
                return -1;
            }
        }
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            if (addr != NULL) {
                *addr = *rp->ai_addr;
            }
            if (addrlen != NULL) {
                *addrlen = rp->ai_addrlen;
            }
            break;
        }
        close(sfd);
    }
    if (rp != NULL && doListen) {
        if (listen(sfd, backlog) == -1) {
            freeaddrinfo(result);
            return -1;
        }
    }
    freeaddrinfo(result);

    return rp == NULL ? -1 : sfd;
}




