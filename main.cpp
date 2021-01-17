#include <iostream>
#include <stdlib.h>
#include <memory>
#include <unistd.h>
#include "base/Socket.h"
#include "base/InetAddr.h"
#include "base/Buffer.h"
#include "server/codec.h"
#include "base/sockets.h"
int32_t getAsInt32(void *ptr) { return *static_cast<int32_t*>(ptr); }

int main() {
    int myerrno;
    std::string ret;
    std::cout << sizeof(int ) << std::endl;
    rain::Buffer buf;
    rain::Socket sockfd;
    rain::InetAddr addr("0.0.0.0", 2021), peerAddr;
    sockfd.bind(addr);
    sockfd.listen();
    auto peerFd = sockfd.accept(&peerAddr);
    std::cout << peerAddr.getIpStr() << ":" <<peerAddr.getPort() << std::endl;
//    while (buf.readFD(peerFd, &myerrno))
//        ret.append(buf.retriveAllAsString()) ;
    auto r = buf.readFdWithSize(peerFd, static_cast<size_t>(sizeof(int32_t)), &myerrno);
    assert(r >= 0);
    int32_t all_size = rain::sockets::host32(*static_cast<int32_t *>((void* )buf.peek()));
    auto r2 = buf.readFdWithSize(peerFd, static_cast<size_t>(all_size), &myerrno);
    assert(r2 == all_size);
    buf.retrieveInt32();
    ret = buf.retriveAllAsString();
    rain::server::codec newcodec(ret);
    newcodec.load();
//    std::cout << newcodec.getSrc() << std::endl;
//    std::cout << newcodec.getLogo() << std::endl;
    newcodec.saveFile();
    return 0;
}
//    std::string srcPtr_, logoPtr_;
//    std::string::iterator it1 = ret.begin(), it2;
//    int32_t head1_32 = getAsInt32(&*it1);
//    int32_t length1_32 = rain::sockets::host32(getAsInt32(&*(it1 + 4)));
//    srcPtr_.assign(&*(it1 + 8), length1_32);
//    it2 = it1 + 4 + 4 + length1_32;
//    int32_t head2_32 = getAsInt32(&*it2);
//    int32_t length2_32 = rain::sockets::host32(getAsInt32(&*(it2 + 4)));
//    logoPtr_.assign(&*(it2 + 8), length2_32);