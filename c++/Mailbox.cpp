//
//  Mailbox.cpp
//
//  This class implements a Mailbox object. A mailbox is associated with a
//  message code and is responsible for dispatching all the telegrams with that
//  message code. Users should not directly interact with this class and should
//  use the MessageDispatcher instead to dispatch messages or add mailboxes.
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


#include "Mailbox.h"
#include <vector>
using namespace cugl;

Mailbox::Mailbox(int tag){
    mailboxTag = tag;
}

/**
 * Updates the mailbox and sends delayed telegrams with an expired timestamp
 * to listeners.
 *
 * @param rtree The R-Tree on which to perform range queries.
 */
void Mailbox::update(std::shared_ptr<RTree> rtree) {
    Timestamp now;
    
    // we pop messages from the queue after the iteration to prevent concurrent modification
    int messagesToBePopped = 0;
    
    for (const std::shared_ptr<Telegram>& msg: messages) {
        Uint64 elapsedMicrosSinceLastUpdate = cugl::Timestamp::ellapsedMicros(msg->lastUpdate, now);
        
        // small optimzation: if we processed this telegram within the last 250 microseconds, return
        // because it is unlikely that we already have expired timestamps. Can be uncommented.
        if (elapsedMicrosSinceLastUpdate < 250) return;

        Uint64 elapsedMillisSinceSent = Timestamp::ellapsedMillis(msg->timeSent, now);
        
        // What was the delay we processed up to when this message was last updated?
        Uint64 lastDelay = Timestamp::ellapsedMillis(msg->timeSent, msg->lastUpdate);

        std::shared_ptr<Telegraph> sender = msg->sender;

        bool allListenersReceived = true;
        // if the sender specified a radius
        if (sender != nullptr && sender->specifiesRadius()) {
            // get all listeners in range of sender's AOI
            std::vector<std::shared_ptr<RTreeObject>> listenersInRange = rtree->search(sender->getCenter(), sender->getRadius(), mailboxTag);
            
            for (auto it = listenersInRange.begin(); it != listenersInRange.end(); it++) {
                // we know only insert Telegraphs into the rtree so this should be a safe cast
                std::shared_ptr<Telegraph> t = std::dynamic_pointer_cast<Telegraph>(*it);
                
                // check if the receiver has a specified radius and if the sender is in the receiver's range
                if (t->specifiesRadius()
                        && !sender->rect.doesIntersect(t->getCenter(), t->getRadius())){
                    continue;
                }

                long delay = delays.at(t);
                
                // check if receiver has received the message
                if (lastDelay > delay && elapsedMillisSinceSent > delay){
                    continue;
                }
                
                // check if receiver hasn't received the message yet
                if(elapsedMillisSinceSent < delay){
                    allListenersReceived = false;
                    continue;
                }
                
                t->handleMessage(msg);
            }
        } else {
            auto it = listeners.upper_bound(lastDelay);
            if(lastDelay == 0){
                it = listeners.begin();
            }
            // otherwise just send to all subscribers
            for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
                if (it->second->specifiesRadius() &&
                    !sender->rect.doesIntersect(it->second->getCenter(), it->second->getRadius()))
                    continue;
                
                it->second->handleMessage(msg);

//            Uncomment the lines below for benchmarking
//            auto measuredDelayMicros = cugl::Timestamp::ellapsedMicros(msg->timeSent, cugl::Timestamp());
//            measuredDelays.emplace_back(measuredDelayMicros - it->first * 1000, it->first);
            }
            if (it != listeners.end()) {
                allListenersReceived = false;
            }
        }

        // if we have sent to all subscribers, pop the message from the queue
        if (allListenersReceived) {
            messagesToBePopped++;
        } else {
            msg->lastUpdate = now;
        }
        
    }

    while (messagesToBePopped > 0) {
        messages.pop_front();
        messagesToBePopped--;
    }
}

/**
 * Directly dispatches a message from the sender to the receiver, without
 * sending it to subscribers of the message code.
 * Caller can optionally attach extra information to the message.
 *
 * The extra information can be of any type, but a destructor should be
 * defined in the class or passed as an argument to the shared_ptr constructor
 * resources can be deleted when the reference count is decremented to zero.
 *
 * @param sender the sender of the message (optional)
 * @param receiver the receiver of the message
 * @param extraInfo optional information, nullptr by default
 */
void Mailbox::dispatchDirectMessage(const std::shared_ptr<Telegraph>&sender,
                              const std::shared_ptr<Telegraph>& receiver,
                              const std::shared_ptr<void>& extraInfo) {
    std::shared_ptr<Telegram> telegram = std::make_shared<Telegram>(extraInfo, sender);
    receiver->handleMessage(telegram);
}

/**
 * Dispatches a message to the listeners subscribed to the code of the mailbox,
 * with a reference to the sender of the message. Caller can optionally
 * attach extra information to the message.
 *
 * The extra information can be of any type, but a destructor should be
 * defined in the class or passed as an argument to the shared_ptr constructor
 * resources can be deleted when the reference count is decremented to zero.
 *
 * @param extraInfo extra information attached to the message. Optional.
 * @param rtree The R-Tree on which to perform range queries.
 * @param sender the sender of the message
 */
void Mailbox::dispatchMessage(const std::shared_ptr<Telegraph>& sender,
                              const std::shared_ptr<RTree> rtree,
                              const std::shared_ptr<void>& extraInfo) {
    std::shared_ptr<Telegram> telegram = std::make_shared<Telegram>(extraInfo, sender);
    
    messages.push_back(telegram);
}

/**
 * Dispatches a message to the listeners subscribed to the message code.
 * Caller can optionally attach extra information to the message.
 *
 * The extra information can be of any type, but a destructor should be
 * defined in the class or passed as an argument to the shared_ptr constructor
 * resources can be deleted when the reference count is decremented to zero.
 *
 * @param rtree The R-Tree on which to perform range queries.
 * @param extraInfo extra information attached to the message. Optional and nullptr by default.
 */
void Mailbox::dispatchMessage(const std::shared_ptr<RTree> rtree, const std::shared_ptr<void>& extraInfo) {
    Mailbox::dispatchMessage(nullptr, rtree, extraInfo);
}

/**
 * Registers a listener with this mailbox. The caller can optionally add
 * a delay (in milliseconds) to the messages that the listener receives from
 * this mailbox.
 *
 * If a listener is already registered, the new delay will replace the old one.
 *
 * @param listener the listener to register
 * @param delay the delay (in milliseconds) on the messages sent to the listener.
 * This is optional and there is no delay by default.
 */
void Mailbox::addListener(const std::shared_ptr<Telegraph>& listener, Uint64 delay) {
    listeners.emplace(delay, listener);
    delays.emplace(listener, delay);
}

/**
 * Unregister the specified listener from this mailbox. This operation
 * is a no-op if the listener is not subscribed to the mailbox.
 *
 * @param listener the listener to remove
 * */
void Mailbox::removeListener(const std::shared_ptr<Telegraph>& listener) {
    if (delays.find(listener) == delays.end()) return;

    Uint64 delay = delays[listener];
    // there could be multiple listeners with the same delay
    auto range = listeners.equal_range(delay);
    // iterate to see which listener we need to remove
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == listener) {
            listeners.erase(it);
            break;  // Stop after removing the first matching element
        }
    }

    delays.erase(listener);
}


