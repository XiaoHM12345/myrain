//
// Created by 84000 on 2021/1/9.
// JetBrains Clion
//

#ifndef RAINNETWORK_INETADDR_H
#define RAINNETWORK_INETADDR_H
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> //bzero
#include <string>

#include "miscellaneous.h"
#include "sockets.h"

RAIN_BEGIN_
class InetAddr {
public:
    InetAddr() { bzero(&addr_, static_cast<size_t>(sizeof(addr_))); }
    InetAddr(std::string ip, uint16_t port)
    {
        bzero(&addr_, static_cast<size_t>(sizeof(addr_)));
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = rain::sockets::netInetAddr(ip);
        addr_.sin_port = rain::sockets::netPort16(port);
    }
    InetAddr(const struct sockaddr_in& addr) :addr_(addr) {}

    sockaddr_in getSocketAddr() { return addr_; }
    sockaddr_in getSocketAddr() const { return addr_; }
    std::string getIpStr() const { return std::string(inet_ntoa(this->addr_.sin_addr)); }
    uint16_t getPort() const { return rain::sockets::hostPort16(this->addr_.sin_port); }

    InetAddr& operator= (const InetAddr& rhs) { this->addr_ = rhs.addr_; }
private:
    struct sockaddr_in addr_;
};



RAIN_END_

#endif //RAINNETWORK_INETADDR_H
