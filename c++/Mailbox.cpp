//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"
#include <vector>

void Mailbox::update() {
    cugl::Timestamp now;
    int messagesToBePopped = 0;
    for (Telegram& msg: messages) {
        // if we processed this telegram within the last 0.5ms, return
        Uint64 elapsedMillisSinceLastUpdate = cugl::Timestamp::ellapsedMillis(msg.lastDelay, now);
        if (elapsedMillisSinceLastUpdate < 1) continue;

        Uint64 elapsedMillisSinceSent = cugl::Timestamp::ellapsedMillis(msg.timeSent, now);

        auto it = listeners.upper_bound(elapsedMillisSinceLastUpdate);
        for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
            it->second.handleMessage(msg);
            auto measuredDelayMicros = cugl::Timestamp::ellapsedMicros(msg.timeSent, cugl::Timestamp());
            measuredDelays.emplace_back(measuredDelayMicros - it->first, it->first);
        }

        if (it == listeners.end()) {
            messagesToBePopped++;
        }
        msg.lastDelay = now;
    }

    while (messagesToBePopped > 0) {
        messages.pop_front();
        messagesToBePopped--;
    }
}

void Mailbox::dispatchMessage(Telegraph &sender, Telegraph &receiver) {


}
void Mailbox::dispatchMessage() {
    Telegram telegram;

    for (const Telegraph& t: immediate) {
        t.handleMessage(telegram);
    }

    messages.push_back(telegram);
}

void Mailbox::dispatchMessage(Telegraph &sender) {
    Mailbox::dispatchMessage();
}

void Mailbox::addListener(Telegraph &listener, Uint64 delay) {
    listeners.emplace(delay, listener);

    if (delay <= 0) {
        immediate.emplace(listener);
    } else {
        delays.emplace(listener, delay);
    }
}

void Mailbox::removeListener(Telegraph &listener) {
    if (delays.find(listener) == delays.end()) return;

    Uint64 delay = delays[listener];
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

    delays.erase(listener);
}


