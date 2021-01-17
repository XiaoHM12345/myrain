//
// Created by 84000 on 2021/1/11.
//
#include <algorithm>
#include <iostream>
#include "codec.h"

void rain::server::codec::load() {
    std::cout << ptr_.size() << std::endl;
    Iterator it1 = ptr_.begin(), it2;
    int32_t head1_32 = getAsInt32(&*it1);
    int32_t length1_32 = rain::sockets::host32(getAsInt32(&*(it1 + 4)));
    srcStr.assign(&*(it1 + 8), length1_32);
    std::cout << srcStr.size() << std::endl;
    it2 = it1 + 4 + 4 + length1_32;
    char* str1 = &*it1;
    int32_t head2_32 = getAsInt32(&*it2);
    int32_t length2_32 = rain::sockets::host32(getAsInt32(&*(it2 + 4)));
    logoStr.assign(&*(it2 + 8), length2_32);
}

std::string rain::server::codec::getSrc() {
    return this->srcStr;
}

std::string rain::server::codec::getLogo() {
    return this->logoStr;
}

void rain::server::codec::saveFile() {
    std::ofstream file1("src.png");
    file1 << srcStr;
    std::ofstream file2("logo.png");
    file2 << logoStr;
    file1.close();
    file2.close();
}
