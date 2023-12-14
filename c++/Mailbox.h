//
//  Mailbox.h
//
//  This class implements a Mailbox object. A mailbox is associated with a
//  message code and is responsible for dispatching all the telegrams with that
//  message code. Users should not directly interact with this class and should
//  use the MessageDispatcher instead.
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

    /**
     * Updates the mailbox and sends delayed telegrams with an expired timestamp
     * to listeners.
     */
    void update();


    static void dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<Telegraph>& receiver, const std::shared_ptr<void>& extraInfo = nullptr);
    void dispatchMessage(const std::shared_ptr<Telegraph>& sender, const std::shared_ptr<void>& extraInfo = nullptr);
    void dispatchMessage(const std::shared_ptr<void>& extraInfo = nullptr);

    /**
     * Registers a listener with this mailbox. The caller can optionally add
     * a delay (in milliseconds) to the messages that the listener receives from
     * this mailbox.
     *
     * @param listener the listener to register
     * @param delay the delay (in milliseconds) on the messages sent to the listener.
     * This is optional and there is no delay by default.
     */
    void addListener(const std::shared_ptr<Telegraph>& listener, Uint64 delay = 0);

    /** Unregister the specified listener from this mailbox.
     *
	 * @param listener the listener to remove
     * */
    bool removeListener(const std::shared_ptr<Telegraph>&);

    /// (for benchmarking) used to record the measured unwanted delays between
    /// the time a message is dispatched and when it is actually sent to a
    /// listener after subtracting the intended delay.
//    std::vector<Delay> measuredDelays;

private:
    /// maps listeners to their delays if the delays are > 0
    std::unordered_map<Telegraph*, Uint64> delays;

    /// maps delays in milliseconds to the set of listeners that have those delays
    std::multimap<Uint64, std::shared_ptr<Telegraph>> listeners;

    /// listeners with a 0ms or unspecified delay. Kept as a separate data structure for optimization.
    std::unordered_set<std::shared_ptr<Telegraph>> immediate;

    /// a FIFO queue for messages in the order that they are first created.
    /// Messages are popped once there are no more deliveries to be made.
    std::deque<std::shared_ptr<Telegram>> messages;

};
#endif //LIBGDX_MAILBOX_H
