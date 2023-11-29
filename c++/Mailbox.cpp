//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"
#include <vector>

using namespace  std::chrono;

static int elapsedMillis(const time_point<system_clock>& start, const time_point<system_clock>& end) {
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    return elapsed;
}

void Mailbox::update() {
    auto now = std::chrono::system_clock::now();
    int messagesToBePopped = 0;
    for (Telegram& msg: messages) {
        // if we processed this telegram within the last 0.5ms, return
        Uint64 elapsedMillisSinceSent = elapsedMillis(msg.timeSent, now);
        Uint64 elapsedMillisSinceLast = elapsedMillis(msg.lastDelay, now);
        if (elapsedMillisSinceLast < 1) continue;
        Uint64 lastCheckpoint = elapsedMillis(msg.timeSent, msg.lastDelay);
        auto it = listeners.upper_bound(lastCheckpoint);
        for (; it != listeners.end() && it->first <= elapsedMillisSinceSent; it++) {
            it->second.handleMessage(msg);
            auto curr = std::chrono::system_clock::now();
            Uint64 diff = elapsedMillis(msg.timeSent, curr) - it->first;
            measuredDelays.emplace_back( diff, it->first);
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

    int delay = delays[listener];
    if (delay <= 0.0) {
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


