cmake_minimum_required(VERSION 3.17)
project(rainNetwork)

set(CMAKE_CXX_STANDARD 14)

add_executable(rainNetWork main.cpp base/InetAddr.h base/miscellaneous.h base/myException.h base/sockets.h base/Socket.cpp base/Socket.h base/sockets.cpp base/Buffer.cpp base/Buffer.h server/codec.cpp server/codec.h)