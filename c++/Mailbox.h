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
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<Telegraph>& receiver);
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender);
    void dispatchMessage(const std::shared_ptr<void>& extraInfo = nullptr);

    void addListener(const std::shared_ptr<Telegraph>& listener, Uint64 delay = 0);
    bool removeListener(const std::shared_ptr<Telegraph>&);
    std::vector<Delay> measuredDelays;
//
//    ~Mailbox() {
//        std::cout << "mailbox destroyed \n";
//    }

private:
    std::unordered_map<Telegraph*, Uint64> delays;
    std::multimap<Uint64, std::shared_ptr<Telegraph>> listeners;
    std::unordered_set<std::shared_ptr<Telegraph>> immediate;
    std::deque<std::shared_ptr<Telegram>> messages;

};
#endif //LIBGDX_MAILBOX_H
