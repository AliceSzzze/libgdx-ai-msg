//
//  Mailbox.h
//
//  This class implements a Mailbox object. A mailbox is associated with a
//  message code and is responsible for dispatching all the telegrams with that
//  message code. Users should not directly interact with this class and should
//  use the MessageDispatcher instead to dispatch messages or add mailboxes.
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

#ifndef CUGL_MAILBOX_H
#define CUGL_MAILBOX_H


#include "Telegraph.h"
#include "Delay.h"
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <queue>
#include "rtree.h"
class Mailbox {
public:
    /**
     * Creates a mailbox.
     */
    Mailbox(int tag);

    /**
     * Updates the mailbox and sends delayed telegrams with an expired timestamp
     * to listeners.
     *
     * @param rtree The R-Tree on which to perform range queries.
     */
    void update(std::shared_ptr<RTree> rtree);

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
     * @param extraInfo optional information, nullptr by default
     */
    void dispatchDirectMessage(const std::shared_ptr<Telegraph>& sender,
                         const std::shared_ptr<Telegraph>& receiver,
                         const std::shared_ptr<void>& extraInfo = nullptr);

    /**
     * Dispatches a message to the listeners subscribed to the code of the mailbox,
     * with a reference to the sender of the message. Caller can optionally
     * attach extra information to the message.
     *
     * The extra information can be of any type, but a destructor should be
     * defined in the class or passed as an argument to the shared_ptr constructor
     * resources can be deleted when the reference count is decremented to zero.
     *
     * @param extraInfo extra information attached to the message. Optional.
     * @param sender the sender of the message
     * @param rtree The R-Tree on which to perform range queries.
     */
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender,
                         const std::shared_ptr<RTree> rtree,
                         const std::shared_ptr<void>& extraInfo = nullptr);

    /**
     * Dispatches a message to the listeners subscribed to the message code.
     * Caller can optionally attach extra information to the message.
     *
     * The extra information can be of any type, but a destructor should be
     * defined in the class or passed as an argument to the shared_ptr constructor
     * resources can be deleted when the reference count is decremented to zero.
     *
     * @param extraInfo extra information attached to the message. Optional and nullptr by default.
     */
    void dispatchMessage(const std::shared_ptr<RTree> rtree, const std::shared_ptr<void>& extraInfo = nullptr);

    /**
     * Registers a listener with this mailbox. The caller can optionally add
     * a delay (in milliseconds) to the messages that the listener receives from
     * this mailbox.
     *
     * If a listener is already registered, the new delay will replace the old one.
     *
     * @param listener the listener to register
     * @param delay the delay (in milliseconds) on the messages sent to the listener.
     * This is optional and there is no delay by default.
     */
    void addListener(const std::shared_ptr<Telegraph>& listener, Uint64 delay = 0);

    /** Unregister the specified listener from this mailbox. This operation
     * is a no-op if the listener is not subscribed to the mailbox.
     *
     * The user should remove listeners that are no longer active.
     *
	 * @param listener the listener to remove
     * */
    void removeListener(const std::shared_ptr<Telegraph>&);

    /// (for benchmarking) used to record the measured unwanted delays between
    /// the time a message is dispatched and when it is actually sent to a
    /// listener after subtracting the intended delay.
//    std::vector<Delay> measuredDelays;

private:
    /// The tag corresponding to this Mailbox.
    int mailboxTag;
    
    /// maps listeners to their delays if the delays are > 0
    std::unordered_map<std::shared_ptr<Telegraph>, Uint64> delays;

    /// maps delays in milliseconds to the set of listeners that have those delays
    std::multimap<Uint64, std::shared_ptr<Telegraph>> listeners;

    /// a FIFO queue for messages in the order that they are first created.
    /// Messages are popped once there are no more deliveries to be made.
    std::deque<std::shared_ptr<Telegram>> messages;
};
#endif //CUGL_MAILBOX_H
