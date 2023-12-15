//
//  MessageDispatcher.h
//
//  This class implements a MessageDispatcher object.
//
//  CUGL MIT License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Alice Sze
//  Version: 12/14/2023
//

#ifndef LIBGDX_MESSAGEDISPATCHER_H
#define LIBGDX_MESSAGEDISPATCHER_H


#include "Mailbox.h"
#include "Telegraph.h"
#include "rtree/rtree.h"
#include <unordered_map>

class MessageDispatcher {
public:
    MessageDispatcher(float x, float y, float width, float height);
    /**
     * Calls update on every mailbox, which then sends delayed telegrams with an
     * expired timestamp to listeners.
     *
     * This method should be called regularly to ensure that delayed messages
     * are dispatched in a timely manner.
     */
    void update();

    /**
     * Adds a new mailbox with the given message code. If a mailbox with the code
     * already exist, the call will be a no-op.
     *
     * @param msg the message code
     */
    void addMailbox(int msg);

    /**
     * Removes the mailbox with the given message code.
     *
     * @param msg the message code
     */
    void removeMailbox(int msg);

    void dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<Telegraph>& receiver, int msg);
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender, int msg);

    /**
     * Dispatches a message with the given code to the listeners subscribed to
     * the code. Caller can optionally attach extra information to the message.
     *
     * The extra information can be of any type, but a destructor should be
     * defined in the class or passed as an argument to the shared_ptr constructor
     * resources can be deleted when the reference count is decremented to zero.
     *
     * @param msg the message code
     * @param extraInfo extra information attached to the message. Optional.
     */
    void dispatchMessage(int msg, const std::shared_ptr<void>& extraInfo = nullptr);

    void addListener(const std::shared_ptr<Telegraph>& listener, int msg, int delay = 0);

    /**
     * Removes the listener as a subscriber to the message code. They should no
     * longer receive messages with that code, even if they were dispatched at
     * when the listener was added.
     *
     * @param listener the listener to remove
     * @param msg the message code to remove the listener from
     * @return whether the listener has been removed
     */
    bool removeListener(const std::shared_ptr<Telegraph>& listener, int msg);

private:
    std::unordered_map<int, std::shared_ptr<Mailbox>> mailboxes;
    std::shared_ptr<RTree> rtree;

};


#endif //LIBGDX_MESSAGEDISPATCHER_H
