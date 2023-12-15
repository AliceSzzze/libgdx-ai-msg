//
//  Telegraph.h
//
//  This class implements a Telegraph object. Only telegraphs can receive messages
//  from the MessageDispatcher, leave sender information in the messages or specify
//  send or receive ranges.
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
#include "rtreeobject.h"
#include <functional>
#include <iostream>

class Telegraph : public RTreeObject {
private:
    /// optional
    cugl::Vec2 center;

    /// optional
    float radius;
    
public:
    /**
     * Creates and initializes a Telegraph object without specifying a send/receive range.
     *
     * @param x The initial x-coordinates of the telegraph
     * @param y The initial y-coordinates of the telegraph
     * @param width The width of the telegraph
     * @param height The height of the telegraph
     */
    Telegraph(float x, float y, float width, float height) : RTreeObject(x,y,width,height) {}
    
    /**
     * Creates and initializes a Telegraph object with a specified send/receive
     * range. The range is a circle that defines who this telegraph can send
     * messages to or receive messages from regardless of tags.
     *
     * @param x The initial x-coordinates of the telegraph
     * @param y The initial y-coordinates of the telegraph
     * @param width The width of the telegraph
     * @param height The height of the telegraph
     * @param center The center of the circular send/receive range
     * @param radius The radius of the circular send/receive range
     */
    Telegraph(float x, float y, float width, float height,cugl::Vec2& center, float radius) : RTreeObject(x,y,width,height){
        this->center = center;
        this->radius = radius;
    }

    cugl::Vec2& getCenter() {
        return center;
    }
    
    float getRadius() {
        return radius;
    }

    /**
     * An abstract method that is invoked when a message is delivered to the Telegraph.
     * Objects that inherit from this class should override this method with their
     * own handler.
     *
     * @param msg The message delivered to the telegraph
     */
    virtual void handleMessage(const std::shared_ptr<Telegram> msg) const {
     };

    virtual bool operator==(const Telegraph& other) const {
        return this == &other;
    }
};

#endif //LIBGDX_TELEGRAPH_H
