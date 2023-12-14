//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"

void MessageDispatcher::update() {
    for (auto &[msg, mailbox] : mailboxes) {
        mailbox->update();
    }
}

void MessageDispatcher::addMailbox(int msg) {
    if (mailboxes.find(msg) == mailboxes.end()) {
        mailboxes.emplace(msg, std::make_shared<Mailbox>());
    }
}

void MessageDispatcher::removeMailbox(int msg) {
    mailboxes.erase(msg);
}

void MessageDispatcher::dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<Telegraph>& receiver, int msg) {
    mailboxes[msg]->dispatchMessage(sender, receiver);
}

void MessageDispatcher::dispatchMessage(const std::shared_ptr<Telegraph>& sender, int msg) {
    mailboxes[msg]->dispatchMessage(sender);
}

void MessageDispatcher::dispatchMessage(int msg, const std::shared_ptr<void>& extraInfo) {
    mailboxes[msg]->dispatchMessage(extraInfo);
}

void MessageDispatcher::addListener(const std::shared_ptr<Telegraph>& listener, int msg, int delay) {
    mailboxes[msg]->addListener(listener, delay);

}

bool MessageDispatcher::removeListener(const std::shared_ptr<Telegraph>& listener, int msg) {
    return mailboxes[msg]->removeListener(listener);
}