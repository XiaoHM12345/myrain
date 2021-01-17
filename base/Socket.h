//
// Created by 84000 on 2021/1/9.
// JetBrains Clion
//

#ifndef RAINNETWORK_SOCKET_H
#define RAINNETWORK_SOCKET_H
#include <sys/socket.h>

#include "miscellaneous.h"
#include "InetAddr.h"

RAIN_BEGIN_

class Socket {
public:
public:
    Socket()
        : sockFd_(socket(AF_INET, SOCK_STREAM, 0))
    { }

    void bind(const InetAddr& addr) const;
    void listen(int backlog = 5) const;
    int accept(InetAddr* peerAddr) const;
private:
    int sockFd_;
};

RAIN_END_


#endif //RAINNETWORK_SOCKET_H
