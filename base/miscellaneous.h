//
// Created by 84000 on 2021/1/9.
// JetBrains Clion
//

#ifndef RAINNETWORK_MISCELLANEOUS_H
#define RAINNETWORK_MISCELLANEOUS_H
#define RAIN_BEGIN_ namespace rain {
#define RAIN_END_ }
#define SOCK_BEGIN_ namespace sockets {
#define SOCK_END_ }
#define INETADDR_BEGIN_ namespace inet_addrs {
#define INETADDR_END_ }
#define SERVER_BEGIN_ namespace server {
#define SERVER_END_ }

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
    return f;
}

#endif //RAINNETWORK_MISCELLANEOUS_H
