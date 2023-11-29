//
// Created by Alice Sze on 11/14/23.
//

#ifndef LIBGDX_MESSAGEDISPATCHER_H
#define LIBGDX_MESSAGEDISPATCHER_H


#include "Mailbox.h"
#include "Telegraph.h"
#include <unordered_map>

class MessageDispatcher {
public:

    void update();
    void addMailbox(int msg);
    void removeMailbox(int msg);

    // TODO: add extra data? delay? sender?
    void dispatchMessage(Telegraph& sender, Telegraph& receiver, int msg);
    void dispatchMessage(Telegraph& sender, int msg);
    void dispatchMessage(int msg);
    void addListener(Telegraph& listener, int msg, int delay = 0);
    void removeListener(Telegraph& listener, int msg);

//private:
    std::unordered_map<int, Mailbox> mailboxes;

};


#endif //LIBGDX_MESSAGEDISPATCHER_H
