//
// Created by Alice Sze on 11/14/23.
//


#include "Mailbox.h"
#include <vector>

void Mailbox::update() {
    cugl::Timestamp now;
    int messagesToBePopped = 0;
    for (const std::shared_ptr<Telegram>& msg: messages) {
        Uint64 elapsedMicrosSinceLastUpdate = cugl::Timestamp::ellapsedMicros(msg->lastUpdate, now);
        // if we processed this telegram within the last 250ms, return

        if (elapsedMicrosSinceLastUpdate < 250) return;

        Uint64 elapsedMillisSinceSent = cugl::Timestamp::ellapsedMillis(msg->timeSent, now);
        Uint64 lastCheckpoint = cugl::Timestamp::ellapsedMillis(msg->timeSent, msg->lastUpdate);
        auto it = listeners.upper_bound(lastCheckpoint);

        std::vector<std::shared_ptr<RTreeObject>> objects = rtree->search(nullptr, 0.0);

        for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
            // if in range?
            for (const auto& obj : objects) {
                if (obj.get() == it->second.get()) {
                    
                }
            }

//                if ()
                    // send if sender is in receiver's range
            it->second->handleMessage(msg);


//            Uncomment the lines below for benchmarking
//            auto measuredDelayMicros = cugl::Timestamp::ellapsedMicros(msg->timeSent, cugl::Timestamp());
//            measuredDelays.emplace_back(measuredDelayMicros - it->first * 1000, it->first);
        }

        if (it == listeners.end()) {
            messagesToBePopped++;
        }
        msg->lastUpdate = now;
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
        t->handleMessage(telegram);
    }

    messages.push_back(telegram);
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


