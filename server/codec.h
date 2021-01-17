//
// Created by 84000 on 2021/1/11.
// JetBrains Clion
//

//Preliminary design: Delegate a buffer class to read the data in the
//socket file descriptor, and then decode two image data, which are
//stored in two std::string. The codec must be destroyed every
//time it is used and cannot be reused.
#ifndef RAINNETWORK_CODEC_H
#define RAINNETWORK_CODEC_H
#include <memory>
#include <string>
#include <fstream>

#include "../base/miscellaneous.h"
#include "../base/Buffer.h"
#include "../base/myException.h"
#include "../base/sockets.h"

//TCP protocol application layer data packet
//____________________________________________________________
//|head(32bytes)|length(32bytes)|data|parity bit(optional)|
//------------------------------------------------------------
RAIN_BEGIN_
SERVER_BEGIN_

class codec {
    typedef std::string::iterator Iterator;
public:
    explicit codec(const std::string& ptr)
        :ptr_(ptr)
    {}
    void load();
    void saveFile();
    std::string getSrc();
    std::string getLogo();
private:
    static int8_t getAsInt8(void *ptr) { return *static_cast<int8_t*>(ptr); }
    static int16_t getAsInt16(void *ptr) { return *static_cast<int16_t*>(ptr); }
    static int32_t getAsInt32(void *ptr) { return *static_cast<int32_t*>(ptr); }
    static int64_t getAsInt64(void *ptr) { return *static_cast<int64_t*>(ptr); }
    //rain::Buffer *buf_;
    std::string ptr_;
    std::string srcStr;
    std::string logoStr;
};

RAIN_END_
SERVER_END_


#endif //RAINNETWORK_CODEC_H
