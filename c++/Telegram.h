//
//  Telegram.h
//
//  This class implements a Telegram object. Telegrams are messages that are created
//  by MessageDispatcher. They can be sent to one or more Telegraphs/subscribers
//  through the dispatcher.
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

#ifndef CUGL_TELEGRAM_H
#define CUGL_TELEGRAM_H

#include <chrono>
#include <utility>

#include <memory>
#include <cugl/cugl.h>

using namespace cugl;

// forward declaration of the Telegraph class to eliminate circular include
// dependency.
class Telegraph;
class Telegram {

public:
    /**
     * Creates and initializes a Telegram.
     *
     * The time this telegram is sent and the last time this telegram has been
     * checked for deliveries are set as the current time.
     */
     Telegram() {
        timeSent = Timestamp();
        lastUpdate = Timestamp(timeSent);
    }

    /**
     * Creates a Telegram with extra information.
     *
     * The extra information can be of any type, but must have a destructor if
     * any resources need to be freed.
     *
     * @param extraInfo Extra information that is attached to the telegram
     */
    explicit Telegram(std::shared_ptr<void> extraInfo) : Telegram() {
        this->extraInfo = std::move(extraInfo);
    }

    /**
     * Creates a Telegram with information about the sender.
     *
     * @param sender The sender of this Telegram.
     */
    explicit Telegram(const std::shared_ptr<Telegraph>& sender) : Telegram() {
        this->sender = sender;
    }

    /**
     * Creates a Telegram with extra information and information about the sender.
     *
     * The extra information can be of any type, but must have a destructor if
     * any resources need to be freed.
     *
     * @param extraInfo Extra information that is attached to the telegram
     * @param sender The sender of this Telegram.
     */
    Telegram(std::shared_ptr<void> extraInfo, const std::shared_ptr<Telegraph>& sender) : Telegram() {
        this->extraInfo = std::move(extraInfo);
        this->sender = sender;
    }

    /// the time at which dispatchMessage is called by the sender and this telegram is created.
    Timestamp timeSent;

    /// the last time we checked this telegram for messages to dispatch.
    /// Used to keep track of which listeners we still have to dispatch to.
    Timestamp lastUpdate;

    /// optional extra information that is associated with this telegram.
    std::shared_ptr<void> extraInfo = nullptr;

    /// sender of this telegram. Optional.
    std::shared_ptr<Telegraph> sender = nullptr;
};


#endif //CUGL_TELEGRAM_H
