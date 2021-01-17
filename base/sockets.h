//
// Created by 84000 on 2021/1/9.
// JetBrains Clion
//

#ifndef RAINNETWORK_SOCKETS_H
#define RAINNETWORK_SOCKETS_H
#include <arpa/inet.h>
#include <string>
#include <netinet/in.h>

#include "miscellaneous.h"
#include "myException.h"
//include "InetAddr.h"

RAIN_BEGIN_
SOCK_BEGIN_

in_addr_t netInetAddr(const char* str);
in_addr_t netInetAddr(const std::string& str);

uint16_t netPort16 (uint16_t port);
uint16_t hostPort16 (uint16_t port);

uint16_t net16(uint16_t n);
uint32_t net32(uint32_t n);

uint16_t host16(uint16_t n);
uint32_t host32(uint32_t n);

void s_bind(int sockFd, const struct sockaddr_in* addr);
void s_listen(int sockFd, int backlog = 5);
int s_accept_block(int sockFd, struct sockaddr_in* addr);
ssize_t s_read(int sockfd, void *buf, size_t count);
ssize_t s_readv(int sockfd, const struct iovec *iov, int iovcnt);

SOCK_END_
RAIN_END_
#endif //RAINNETWORK_SOCKETS_H
