//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"

MessageDispatcher::MessageDispatcher(float x, float y, float width, float height) {
    rtree = std::make_shared<RTree>(x, y, width, height);
}
void MessageDispatcher::update() {
    rtree->update();
    for (auto &[msg, mailbox] : mailboxes) {
        mailbox->update();
    }
}

void MessageDispatcher::addMailbox(int msg) {
    if (mailboxes.find(msg) == mailboxes.end()) {
        mailboxes.emplace(msg, std::make_shared<Mailbox>(rtree));
    }
}

void MessageDispatcher::removeMailbox(int msg) {
    mailboxes.erase(msg);
}

void MessageDispatcher::dispatchMessage(const std::shared_ptr<Telegraph>& receiver, int msg, const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(sender, receiver, extraInfo);
}

void MessageDispatcher::dispatchMessage(int msg, const std::shared_ptr<Telegraph>& sender,  const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(sender);
}

void MessageDispatcher::dispatchMessage(int msg, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(extraInfo);
}

void MessageDispatcher::addListener(const std::shared_ptr<Telegraph>& listener, int msg, int delay) {
    mailboxes.at(msg)->addListener(listener, delay);
}

void MessageDispatcher::removeListener(const std::shared_ptr<Telegraph>& listener, int msg) {
    return mailboxes.at(msg)->removeListener(listener);
}
