//
//  MessageDispatcher.cpp
//
//  This class implements a MessageDispatcher object. A MessageDispatcher serves
//  as a central post office that is responsible for getting messages from senders
//  / publishers to the receivers/subscribers. This keeps the two sides decoupled.
//  The dispatcher should be updated regularly in a main game loop.
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

#include "MessageDispatcher.h"

MessageDispatcher::MessageDispatcher(float x, float y, float width, float height, int rTreeMaxPerLevel, int rTreeMinPerLevel, int rTreePadding) {
    rtree = std::make_shared<RTree>(x, y, width, height, rTreeMaxPerLevel, rTreeMinPerLevel, rTreePadding);
}

/**
 * Calls update on every mailbox, which then sends delayed telegrams with an
 * expired timestamp to listeners.
 *
 * This method should be called regularly to ensure that delayed messages
 * are dispatched in a timely manner.
 */
void MessageDispatcher::update() {
    rtree->update();
    for (auto &[msg, mailbox] : mailboxes) {
        mailbox->update(rtree);
    }
}

/**
 * Adds a new mailbox with the given message code. If a mailbox with the code
 * already exist, the call will be a no-op.
 *
 * @param msg the message code
 */
void MessageDispatcher::addMailbox(int msg) {
    if (mailboxes.find(msg) == mailboxes.end()) {
        mailboxes.emplace(msg, std::make_shared<Mailbox>(msg));
    }
}

/**
 * Removes the mailbox with the given message code.
 *
 * @param msg the message code
 */
void MessageDispatcher::removeMailbox(int msg) {
    mailboxes.erase(msg);
}

/**
 * Directly dispatches a message from the sender to the receiver, without
 * sending it to subscribers of the message code.
 * Caller can optionally attach extra information to the message.
 *
 * The extra information can be of any type, but a destructor should be
 * defined in the class or passed as an argument to the shared_ptr constructor
 * resources can be deleted when the reference count is decremented to zero.
 *
 * @param sender the sender of the message (optional)
 * @param receiver the receiver of the message
 * @param msg the message code
 * @param extraInfo optional information, nullptr by default
 */
void MessageDispatcher::dispatchDirectMessage(const std::shared_ptr<Telegraph>& receiver, int msg, const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchDirectMessage(sender, receiver, extraInfo);
}

/**
 * Dispatches a message with the given code to the listeners subscribed to
 * the code, with a reference to the sender of the message. Caller can
 * optionally attach extra information to the message.
 *
 * The extra information can be of any type, but a destructor should be
 * defined in the class or passed as an argument to the shared_ptr constructor
 * resources can be deleted when the reference count is decremented to zero.
 *
 * @param sender the sender of the message
 * @param msg the message code
 * @param extraInfo extra information attached to the message. Optional.
 */
void MessageDispatcher::dispatchMessage(const std::shared_ptr<Telegraph>& sender, int msg, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(sender, rtree, extraInfo);
}

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
void MessageDispatcher::dispatchMessage(int msg, const std::shared_ptr<void>& extraInfo) {
    mailboxes.at(msg)->dispatchMessage(rtree, extraInfo);
}

/**
 * Registers a listener with the given message code. The caller can optionally add
 * a delay (in milliseconds) to the messages that the listener receives with
 * this message code.
 *
 * @param listener the listener to register
 * @param msg the message code to subscribe to
 * @param delay the delay (in milliseconds) on the messages sent to the listener.
 * This is optional and there is no delay by default.
 */
void MessageDispatcher::addListener(const std::shared_ptr<Telegraph>& listener, int msg, int delay) {
    mailboxes.at(msg)->addListener(listener, delay);
    listener->addTag(msg);
    rtree->insert(listener);
}

/**
 * Removes the listener as a subscriber to the message code. They should no
 * longer receive messages with that code, even if they were dispatched at
 * when the listener was added.
 *
 * @param listener the listener to remove
 * @param msg the message code to remove the listener from
 */
void MessageDispatcher::removeListener(const std::shared_ptr<Telegraph>& listener, int msg) {
    listener->removeTag(msg);
    if(!listener->subscribesToTag()){
        rtree->remove(listener);
    }
    return mailboxes.at(msg)->removeListener(listener);
}
