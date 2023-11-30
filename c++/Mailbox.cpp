//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"
#include <vector>

void Mailbox::update() {
    cugl::Timestamp now;
    int messagesToBePopped = 0;
    for (const std::shared_ptr<Telegram>& msg: messages) {
        Uint64 elapsedMicrosSinceLastUpdate = cugl::Timestamp::ellapsedMicros(msg->lastDelay, now);
        // if we processed this telegram within the last 0.5ms, return

        if (elapsedMicrosSinceLastUpdate < 250) return;

        Uint64 elapsedMillisSinceSent = cugl::Timestamp::ellapsedMillis(msg->timeSent, now);
        Uint64 lastCheckpoint = cugl::Timestamp::ellapsedMillis(msg->timeSent, msg->lastDelay);
        auto it = listeners.upper_bound(lastCheckpoint);
        for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
            it->second->handleMessage(msg);
            auto measuredDelayMicros = cugl::Timestamp::ellapsedMicros(msg->timeSent, cugl::Timestamp());
            measuredDelays.emplace_back(measuredDelayMicros - it->first * 1000, it->first);
        }

        if (it == listeners.end()) {
            messagesToBePopped++;
        }
        msg->lastDelay = now;
    }

    while (messagesToBePopped > 0) {
        messages.pop_front();
        messagesToBePopped--;
    }
}

void Mailbox::dispatchMessage(const std::shared_ptr<Telegraph>&sender, const std::shared_ptr<Telegraph>& receiver) {


}
void Mailbox::dispatchMessage(const std::shared_ptr<void>& extraInfo) {
    std::shared_ptr<Telegram> telegram = std::make_shared<Telegram>(extraInfo);

    for (const auto& t: immediate) {
        t->handleMessage(telegram);
    }

    messages.push_back(telegram);
}

void Mailbox::dispatchMessage(const std::shared_ptr<Telegraph>& sender) {
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


