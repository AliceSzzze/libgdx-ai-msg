//
// Created by Alice Sze on 11/14/23.
//

#include "MessageDispatcher.h"

void MessageDispatcher::update() {
    for (auto &[msg, mailbox] : mailboxes) {
        mailbox.update();
    }
}

void MessageDispatcher::addMailbox(int msg) {
    mailboxes.emplace(msg, Mailbox());
}

void MessageDispatcher::removeMailbox(int msg) {
    mailboxes.erase(msg);
}

void MessageDispatcher::dispatchMessage(Telegraph& sender, Telegraph& receiver, int msg) {
    mailboxes[msg].dispatchMessage(sender, receiver);
}

void MessageDispatcher::dispatchMessage(Telegraph& sender, int msg) {
    mailboxes[msg].dispatchMessage(sender);
}

void MessageDispatcher::dispatchMessage(int msg) {
    mailboxes[msg].dispatchMessage();
}

void MessageDispatcher::addListener(Telegraph& listener, int msg, int delay) {
    mailboxes[msg].addListener(listener, delay);

}

void MessageDispatcher::removeListener(Telegraph &listener, int msg) {
    mailboxes[msg].removeListener(listener);
}