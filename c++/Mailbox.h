//
// Created by Alice Sze on 11/14/23.
//

#ifndef LIBGDX_MAILBOX_H
#define LIBGDX_MAILBOX_H


#include "Telegraph.h"
#include "Delay.h"
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <queue>

class Mailbox {
public:
    Mailbox() = default;
    void update();
    void dispatchMessage(Telegraph& sender, Telegraph& receiver);
    void dispatchMessage(Telegraph& sender);
    void dispatchMessage();

    void addListener(Telegraph &listener, Uint64 delay = 0);
    void removeListener(Telegraph& listener);
    std::vector<Delay> measuredDelays;

//    ~Mailbox() {
//        std::cout << "mailbox destroyed \n";
//    }

private:
    std::unordered_map<Telegraph, Uint64> delays;
    std::multimap<Uint64, Telegraph> listeners;
    std::unordered_set<Telegraph> immediate;
    std::deque<Telegram> messages;
//    std::unordered_set<std::pair<Telegram*, >>


};
#endif //LIBGDX_MAILBOX_H
