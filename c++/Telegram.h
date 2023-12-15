//
//  Telegram.h
//
//  This class implements a Telegram object. A Telegram is a message that is sent
//  by a Telegraph. The same Telegram can be sent to one or more Telegraphs through
//  the MessageDispatcher.
//
//  CUGL MIT License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Alice Sze
//  Version: 12/14/2023
//
#ifndef LIBGDX_TELEGRAM_H
#define LIBGDX_TELEGRAM_H
#include "CUTimestamp.h"

#include <chrono>
#include <utility>
#include <memory>
#include <cugl/cugl.h>

using namespace cugl;

class Telegram {

public:
     Telegram() {
        timeSent = cugl::Timestamp();
        lastUpdate = cugl::Timestamp();
    }

    explicit Telegram(std::shared_ptr<void> extraInfo) : Telegram() {
        this->extraInfo = std::move(extraInfo);
    }


    /// the time at which dispatchMessage is called and this telegram is created.
    cugl::Timestamp timeSent;

    /// the last time we checked this telegram for messages to dispatch.
    /// Used to keep track of which listeners we still have to dispatch to.
    cugl::Timestamp lastUpdate;


    /// optional extra information that is associated with this telegram.
    std::shared_ptr<void> extraInfo = nullptr;

    /// center of the sender, optional
    Vec2 center;

    /// radius of the sender, optional
    float radius;
};


#endif //LIBGDX_TELEGRAM_H
