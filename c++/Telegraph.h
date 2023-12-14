//
//  Telegraph.h
//
//  This class implements a Telegraph object. Any object that inherits this class
//  can act as the sender or the receiver of a Telegram.
//
//  Subclasses should override the handleMessage function and equals operator.
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

#ifndef LIBGDX_TELEGRAPH_H
#define LIBGDX_TELEGRAPH_H


#include "Telegram.h"
#include <functional>
#include <iostream>

class Telegraph {
public:
    virtual void handleMessage(const std::shared_ptr<Telegram>& msg) const {
     };

    virtual bool operator==(const Telegraph& other) const {
        return this == &other;
    }
};

#endif //LIBGDX_TELEGRAPH_H
