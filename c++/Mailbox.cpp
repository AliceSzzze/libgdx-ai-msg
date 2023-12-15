//
// Created by Alice Sze on 11/14/23.
//


#include "Mailbox.h"
#include <vector>

void Mailbox::update() {
    cugl::Timestamp now;
    
    // we pop messages from the queue after the iteration to prevent concurrent modification
    int messagesToBePopped = 0;
    
    for (const std::shared_ptr<Telegram>& msg: messages) {
        Uint64 elapsedMicrosSinceLastUpdate = cugl::Timestamp::ellapsedMicros(msg->lastUpdate, now);
        
        // small optimzation: if we processed this telegram within the last 250 microseconds, return
        // because it is unlikely that we already have expired timestamps. Can be uncommented.
        if (elapsedMicrosSinceLastUpdate < 250) return;

        Uint64 elapsedMillisSinceSent = cugl::Timestamp::ellapsedMillis(msg->timeSent, now);
        
        // What was the delay we processed up to when this message was last updated?
        Uint64 lastDelay = cugl::Timestamp::ellapsedMillis(msg->timeSent, msg->lastUpdate);
        
        // we want to get all the listeners with a larger delay than what we last processed
        // because they haven't received the message yet
        auto it = listeners.upper_bound(lastDelay);
        
        std::shared_ptr<Telegraph> sender = msg->sender;

        // if the sender specified a radius
        if (sender != nullptr && sender->getCenter() != nullptr) {
            
            for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
                
                // check if receiver is in sender's range
                if (!rtree->isInRange(sender->getCenter(), sender->getRadius(), it->second.get())) continue;
                
                // check if the receiver has a specified radius and if the sender is in the receiver's range
                if (it->second->getCenter() != nullptr &&
                    !rtree->isInRange(it->second->getCenter(), it->second->getRadius(), sender.get())) continue;
                
                it->second->handleMessage(msg);
            }
        } else {
            // otherwise just send to all subscribers
            for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
                it->second->handleMessage(msg);

//            Uncomment the lines below for benchmarking
//            auto measuredDelayMicros = cugl::Timestamp::ellapsedMicros(msg->timeSent, cugl::Timestamp());
//            measuredDelays.emplace_back(measuredDelayMicros - it->first * 1000, it->first);
            }
        }

        // if we have sent to all subscribers, pop the message from the queue
        if (it == listeners.end()) {
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



void Mailbox::dispatchMessage(const std::shared_ptr<Telegraph>&sender, const std::shared_ptr<Telegraph>& receiver, const std::shared_ptr<void>& extraInfo) {
    std::shared_ptr<Telegram> telegram = std::make_shared<Telegram>(extraInfo);
    receiver->handleMessage(telegram);
}

void Mailbox::dispatchMessage(const std::shared_ptr<void>& extraInfo) {
    std::shared_ptr<Telegram> telegram = std::make_shared<Telegram>(extraInfo);

    for (const auto& t: immediate) {
        // deliver to listeners who do not have a delay right away
        t->handleMessage(telegram);
    }

    if (delays.size() > 0) {
        messages.push_back(telegram);
    }
}

void Mailbox::dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<void>& extraInfo) {
    Mailbox::dispatchMessage();
}

void Mailbox::addListener(const std::shared_ptr<Telegraph>& listener, Uint64 delay) {
    listeners.emplace(delay, listener);

    if (delay <= 0) {
        immediate.emplace(listener);
    } else {
        delays.emplace(listener.get(), delay);
    }
}

bool Mailbox::removeListener(const std::shared_ptr<Telegraph>& listener) {
    if (delays.find(listener.get()) == delays.end()) return false;

    Uint64 delay = delays[listener.get()];
    if (delay <= 0) {
        immediate.erase(listener);
    } else {
        auto range = listeners.equal_range(delay);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == listener) {
                listeners.erase(it);
                break;  // Stop after removing the first matching element
            }
        }
    }

    delays.erase(listener.get());
    return true;
}


