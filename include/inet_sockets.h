#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>

class inet_sockets {
public:
    // init socket, and connect to server(host, service)
    static int inet_connect(const char *host, const char *service, int type);

    // init tcp server socket, and listen
    static int inet_listen(const char *service, int backlog, struct sockaddr *addr, socklen_t *addrlen);

    // init tcp/udp server socket, and bind port
    static int inet_bind(const char *service, int type, struct sockaddr *addr, socklen_t *addrlen);

    // get local addr according to fd
    static int get_sock_addr(int fd, char *addrStr, int addrStrLen);

    // get local addr according to fd
    static int get_sock_addr(int fd, struct sockaddr *addr, socklen_t *addrlen, char *addrStr, int addrStrLen);

    // get peer addr according to fd
    static int get_peer_addr(int fd, char *addrStr, int addrStrLen);

    // get peer addr according to fd
    static int get_peer_addr(int fd, struct sockaddr *addr, socklen_t *addrlen, char *addrStr, int addrStrLen);

    // get addr str according to sockaddr
    static int inet_addr_str(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen);

private:
    static int inet_passive_socket(const char *service, int type, bool doListen, int backlog, struct sockaddr *addr, socklen_t *addrlen);

};

#endif //INET_SOCKETS_H
