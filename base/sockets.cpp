//
// Created by 84000 on 2021/1/9.
//
#include <errno.h>
#include <unistd.h>
#include "sockets.h"
#include <sys/uio.h>

in_addr_t rain::sockets::netInetAddr(const char *str) {
    in_addr_t ret = inet_addr(str);
    if (ret == INADDR_NONE)
        throw rain::myRuntimeErr("invalid ipv4 addr!");
    return ret;
}

in_addr_t rain::sockets::netInetAddr(const std::string &str) {
    in_addr_t ret = inet_addr(str.c_str());
    if (ret == INADDR_NONE)
        throw rain::myRuntimeErr("invalid ipv4 addr!");
    return ret;
}

void rain::sockets::s_bind(int sockFd, const struct sockaddr_in* addr) {
    int ret = ::bind(sockFd, static_cast<const struct sockaddr*>(implicit_cast<const void* >(addr)),
                        static_cast<socklen_t>(sizeof(struct sockaddr_in)));
    if (ret < 0)
        throw rain::myRuntimeErr("bind error!");
}

void rain::sockets::s_listen(int sockFd, int backlog) {
    int ret = ::listen(sockFd, backlog);

    if (ret < 0)
        throw rain::myRuntimeErr("listen error");
}

int rain::sockets::s_accept_block(int sockFd, struct sockaddr_in *addr) {
    socklen_t len = static_cast<socklen_t >(sizeof *addr);
    int ret = ::accept(sockFd, static_cast<struct sockaddr* >(implicit_cast<void* >(addr)), &len);
    int a = errno;
    if (ret < 0)
        throw rain::myRuntimeErr("accept error");
    return ret;
}

uint16_t rain::sockets::hostPort16(uint16_t port) {
    return ntohs(port);
}

uint16_t rain::sockets::netPort16(uint16_t port) {
    return htons(port);
}

ssize_t rain::sockets::s_read(int sockfd, void *buf, size_t count) {
    return ::read(sockfd, buf, count);
}

ssize_t rain::sockets::s_readv(int sockfd, const struct iovec *iov, int iovcnt) {
    return ::readv(sockfd, iov, iovcnt);
}

uint16_t rain::sockets::net16(uint16_t n) {
    return htons(n);
}

uint32_t rain::sockets::net32(uint32_t n) {
    return htonl(n);
}

uint16_t rain::sockets::host16(uint16_t n) {
    return ntohs(n);
}

uint32_t rain::sockets::host32(uint32_t n) {
    return ntohl(n);
}
