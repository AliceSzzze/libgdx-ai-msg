cmake_minimum_required(VERSION 3.26)
project(libgdx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -O3)

include_directories(c++)

add_executable(libgdx
        c++/Mailbox.cpp
        c++/Mailbox.h
        c++/MessageDispatcher.cpp
        c++/MessageDispatcher.h
        c++/Telegraph.h
        c++/Telegram.h
        c++/main.cpp
        c++/Delay.h
        c++/CUTimestamp.h)
