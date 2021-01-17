//
// Created by 84000 on 2021/1/11.
// JetBrains Clion
//

#ifndef RAINNETWORK_BUFFER_H
#define RAINNETWORK_BUFFER_H
#include <vector>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <string>
#include <assert.h>

#include "miscellaneous.h"

RAIN_BEGIN_

class Buffer {
public:
    explicit Buffer(size_t initialSize = kInitialSize)
            : buffer_(kCheapPrepend + initialSize),
              readerIndex_(kCheapPrepend),
              writerIndex_(kCheapPrepend)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initialSize);
        assert(prependableBytes() == kCheapPrepend);
    }

    void swap(Buffer& rhs) //交换两个成员的值，包括内存
    {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    const char* peek() const { return begin() + readerIndex_; }     //可读区域的开始指针

    std::string retriveAsString(size_t len)
    {
        assert(len <= readableBytes());
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }

    std::string retriveAllAsString()
    {
        return retriveAsString(readableBytes());
    }

    // retrieve returns void, to prevent
    // string str(retrieve(readableBytes()), readableBytes());
    // the evaluation of two functions are unspecified
    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        if (len < readableBytes())
        {
            readerIndex_ += len;
        }
        else
        {
            retrieveAll();
        }
    }
    void retrieveUntil(const char* end)
    {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
    }
    void retrieveInt64(){retrieve(sizeof(int64_t));}
    void retrieveInt32() {retrieve(sizeof(int32_t));}
    void retrieveInt16() {retrieve(sizeof(int16_t));}
    void retrieveInt8() {retrieve(sizeof(int8_t));}
    void retrieveAll()
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    char* beginWrite() { return begin() + writerIndex_; }

    //rain::StringInterface toStringInterface() const { return rain::StringInterface(peek(), static_cast<int>(readableBytes()));}

    //向缓冲区中写
    void append(const char* /*restrict*/ data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data+len, beginWrite());
        hasWritten(len);
    }
    //void append(const rain::StringInterface& str) { append(str.data(), str.size());}
    void prepend(const void* /*restrict*/ data, size_t len)
    {
        assert(len <= prependableBytes());
        readerIndex_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d+len, begin()+readerIndex_);
    }

    static const size_t kCheapPrepend = 8;      //预留preappend的大小
    static const size_t kInitialSize = 1024;    //初始化buffer大小

    size_t readableBytes() const {return writerIndex_ - readerIndex_;}
    size_t writableBytes() const {return buffer_.size() - writerIndex_;}
    size_t prependableBytes() const {return readerIndex_;}

    void ensureWritableBytes(size_t len) //确保剩余长度能写
    {
        if (writableBytes() < len)
        {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }
    void hasWritten(size_t len)//增大读区，缩小写区，长度为len
    {
        assert(len <= writableBytes());
        writerIndex_ += len;
    }

    //从文件描述符中直接读取数据到buffer
    //保存了errno
    ssize_t readFD(int fd, int* savedErrno);
    ssize_t readFdWithSize(int fd, size_t n, int* savedErrno);
private:
    char* begin() //vector内存是连续的
    {return &*buffer_.begin();} //先取开始的内存的那一块的值，然后再取指针}

    const char* begin() const {return &*buffer_.begin();}

    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            ///可写空间和移动中腾出的空间是否小于写入长度和预留空间的长度和
            //vector.resize不会擦除原来的数据，而是在后面继续扩张
            //空间不够扩张？会重新挪移块地方或返回错误吧，我也不太清楚
            //最后会重新生成新的len长度可写空间
            buffer_.resize(writerIndex_ + len);
        } else {
            //可以使用read区域前面经过移动过程中腾出来的空间
            assert(kCheapPrepend < readerIndex_); //因为某些情况预留空间不够用了

            //向前挪
            size_t readable = readableBytes(); //可读的字节数
            std::copy(buffer_.begin()+readerIndex_, buffer_.begin()+writerIndex_, buffer_.begin()+kCheapPrepend );
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
            assert(readable == readableBytes());
        }
    }

    std::vector<char> buffer_;  //buffer本体
    size_t readerIndex_; //用来标记可读区域的开始指针
    size_t writerIndex_;  //用来标记可读区域的结束指针

    static const char kCRLF[];
};

RAIN_END_
#endif //RAINNETWORK_BUFFER_H
