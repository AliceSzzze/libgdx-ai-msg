//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"

MessageDispatcher::MessageDispatcher(float x, float y, float width, float height, int rTreeMaxPerLevel, int rTreeMinPerLevel, int rTreePadding) {
    rtree = std::make_shared<RTree>(x, y, width, height, rTreeMaxPerLevel, rTreeMinPerLevel, rTreePadding);
}
void MessageDispatcher::update() {
    rtree->update();
    for (auto &[msg, mailbox] : mailboxes) {
        mailbox->update(rtree);
    }
}

void MessageDispatcher::addMailbox(int msg) {
    if (mailboxes.find(msg) == mailboxes.end()) {
        mailboxes.emplace(msg, std::make_shared<Mailbox>(msg));
    }
}

void MessageDispatcher::removeMailbox(int msg) {
    mailboxes.erase(msg);
}

void MessageDispatcher::dispatchDirectMessage(const std::shared_ptr<Telegraph>& receiver, int msg, const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchDirectMessage(sender, receiver, extraInfo);
}

void MessageDispatcher::dispatchMessage(const std::shared_ptr<Telegraph>& sender, int msg, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(sender, rtree, extraInfo);
}

void MessageDispatcher::dispatchMessage(int msg, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(rtree, extraInfo);
}

void MessageDispatcher::addListener(const std::shared_ptr<Telegraph>& listener, int msg, int delay) {
    mailboxes.at(msg)->addListener(listener, delay);
    listener->addTag(msg);
    rtree->insert(listener);
}

void MessageDispatcher::removeListener(const std::shared_ptr<Telegraph>& listener, int msg) {
    listener->removeTag(msg);
    if(!listener->subscribesToTag()){
        rtree->remove(listener);
    }
    return mailboxes.at(msg)->removeListener(listener);
}
