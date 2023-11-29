//
// Created by Alice Sze on 11/14/23.
//

#ifndef LIBGDX_TELEGRAM_H
#define LIBGDX_TELEGRAM_H
#include <chrono>
#include "CUTimestamp.h"
using namespace  std::chrono;
class Telegram {

public:
     Telegram() {
        timeSent = std::chrono::system_clock::now();
        lastDelay =  std::chrono::system_clock::now();
    }

    time_point<system_clock> timeSent;
    time_point<system_clock> lastDelay;
};


#endif //LIBGDX_TELEGRAM_H
