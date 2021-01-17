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

    void swap(Buffer& rhs) //����������Ա��ֵ�������ڴ�
    {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    const char* peek() const { return begin() + readerIndex_; }     //�ɶ�����Ŀ�ʼָ��

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

    //�򻺳�����д
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

    static const size_t kCheapPrepend = 8;      //Ԥ��preappend�Ĵ�С
    static const size_t kInitialSize = 1024;    //��ʼ��buffer��С

    size_t readableBytes() const {return writerIndex_ - readerIndex_;}
    size_t writableBytes() const {return buffer_.size() - writerIndex_;}
    size_t prependableBytes() const {return readerIndex_;}

    void ensureWritableBytes(size_t len) //ȷ��ʣ�೤����д
    {
        if (writableBytes() < len)
        {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }
    void hasWritten(size_t len)//�����������Сд��������Ϊlen
    {
        assert(len <= writableBytes());
        writerIndex_ += len;
    }

    //���ļ���������ֱ�Ӷ�ȡ���ݵ�buffer
    //������errno
    ssize_t readFD(int fd, int* savedErrno);
    ssize_t readFdWithSize(int fd, size_t n, int* savedErrno);
private:
    char* begin() //vector�ڴ���������
    {return &*buffer_.begin();} //��ȡ��ʼ���ڴ����һ���ֵ��Ȼ����ȡָ��}

    const char* begin() const {return &*buffer_.begin();}

    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            ///��д�ռ���ƶ����ڳ��Ŀռ��Ƿ�С��д�볤�Ⱥ�Ԥ���ռ�ĳ��Ⱥ�
            //vector.resize�������ԭ�������ݣ������ں����������
            //�ռ䲻�����ţ�������Ų�ƿ�ط��򷵻ش���ɣ���Ҳ��̫���
            //�������������µ�len���ȿ�д�ռ�
            buffer_.resize(writerIndex_ + len);
        } else {
            //����ʹ��read����ǰ�澭���ƶ��������ڳ����Ŀռ�
            assert(kCheapPrepend < readerIndex_); //��ΪĳЩ���Ԥ���ռ䲻������

            //��ǰŲ
            size_t readable = readableBytes(); //�ɶ����ֽ���
            std::copy(buffer_.begin()+readerIndex_, buffer_.begin()+writerIndex_, buffer_.begin()+kCheapPrepend );
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
            assert(readable == readableBytes());
        }
    }

    std::vector<char> buffer_;  //buffer����
    size_t readerIndex_; //������ǿɶ�����Ŀ�ʼָ��
    size_t writerIndex_;  //������ǿɶ�����Ľ���ָ��

    static const char kCRLF[];
};

RAIN_END_
#endif //RAINNETWORK_BUFFER_H
