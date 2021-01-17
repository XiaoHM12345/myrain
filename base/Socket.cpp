//
// Created by 84000 on 2021/1/9.
//

#include "Socket.h"

void rain::Socket::bind(const rain::InetAddr &addr) const {
    struct sockaddr_in addr_addr = addr.getSocketAddr();
    rain::sockets::s_bind(this->sockFd_, &addr_addr);
}

void rain::Socket::listen(int backlog) const {
    rain::sockets::s_listen(this->sockFd_, backlog);
}

int rain::Socket::accept(rain::InetAddr *peerAddr) const {
    struct sockaddr_in peerAddr_;
    bzero(&peerAddr_, static_cast<size_t>(sizeof(peerAddr_)));
    int ret = rain::sockets::s_accept_block(this->sockFd_, &peerAddr_);
    InetAddr retAddr(peerAddr_);
    *peerAddr = retAddr;
    return ret;
}



