//
// Created by Alice Sze on 11/14/23.
//

#ifndef LIBGDX_TELEGRAM_H
#define LIBGDX_TELEGRAM_H
#include <chrono>
#include <utility>
#include "CUTimestamp.h"

class Telegram {

public:
     Telegram() {
        timeSent = cugl::Timestamp();
        lastDelay = cugl::Timestamp();
    }

    explicit Telegram(std::shared_ptr<void> extraInfo) : Telegram() {
        this->extraInfo = std::move(extraInfo);
    }

    cugl::Timestamp timeSent;
    cugl::Timestamp lastDelay;
    std::shared_ptr<void> extraInfo;
};


#endif //LIBGDX_TELEGRAM_H
