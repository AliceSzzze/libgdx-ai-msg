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
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<Telegraph>& receiver, int msg);
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender, int msg);
    void dispatchMessage(int msg, const std::shared_ptr<void>& extraInfo = nullptr);
    void addListener(const std::shared_ptr<Telegraph>& listener, int msg, int delay = 0);
    bool removeListener(const std::shared_ptr<Telegraph>& listener, int msg);

//private:
    std::unordered_map<int, std::shared_ptr<Mailbox>> mailboxes;

};


#endif //LIBGDX_MESSAGEDISPATCHER_H
