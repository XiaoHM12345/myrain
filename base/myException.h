//
// Created by 84000 on 2021/1/9.
// JetBrains Clion
//

#ifndef RAINNETWORK_MYEXCEPTION_H
#define RAINNETWORK_MYEXCEPTION_H
#include <stdexcept>
#include <string>

#include "miscellaneous.h"
RAIN_BEGIN_

class myRuntimeErr: public std::runtime_error {
public:
    explicit myRuntimeErr(const std::string &s)
        : std::runtime_error(s)
    { }
};

RAIN_END_



#endif //RAINNETWORK_MYEXCEPTION_H
