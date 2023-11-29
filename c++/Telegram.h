//
// Created by Alice Sze on 11/14/23.
//

#ifndef LIBGDX_TELEGRAM_H
#define LIBGDX_TELEGRAM_H
#include <chrono>
#include "CUTimestamp.h"

class Telegram {

public:
     Telegram() {
        timeSent = cugl::Timestamp();
        lastDelay = cugl::Timestamp();
    }

    cugl::Timestamp timeSent;
    cugl::Timestamp lastDelay;
};


#endif //LIBGDX_TELEGRAM_H
