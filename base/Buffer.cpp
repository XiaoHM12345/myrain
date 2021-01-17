//
// Created by 84000 on 2021/1/11.
//
#include <sys/uio.h>
#include <errno.h>

#include "Buffer.h"
#include "sockets.h"

ssize_t rain::Buffer::readFD(int fd, int *savedErrno) {
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    //确定往哪个缓冲区读
    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    //开始读取数据
    const ssize_t n = rain::sockets::s_readv(fd, vec, iovcnt);

    if (n < 0) {
        *savedErrno = errno;
    } else if (implicit_cast<size_t>(n) <= writable) {
        writerIndex_ += n;
    } else {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }

    return n;
}

ssize_t rain::Buffer::readFdWithSize(int fd, size_t n, int *savedErrno) {
    size_t nleft;
    ssize_t nread;
    nleft = n;
    char ptr[65536];
    while (nleft > 0) {
        if ((nread = rain::sockets::s_read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0)
            break;
        nleft -= nread;
        this->append(ptr, nread);
    }
    return n-nleft;
}
