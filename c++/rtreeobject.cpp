//
//  RTreeObject.cpp
//
//  This class implements an RTreeObject, which is the parent class of anything
//  that will be inserted into an R-Tree. It is defined by a bounding box, and
//  can have various tags to match with queries for the same tag.
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
//  Author: Simon Kapen
//  Version: 12/15/2023
//

#include "rtreeobject.h"
#include <cugl/cugl.h>

using namespace cugl;

RTreeObject::RTreeObject(float x, float y, float width, float height) {
    rect = Rect(x, y, width, height);
}

/**
 * Adds a tag to the set of subscribed tags.
 *
 * @param tag The tag to subscribe to.
 */
void RTreeObject::addTag(int tag){
    tags.insert(tag);
}

/**
 * Removes a tag from the set of subscribed tags.
 *
 * @param tag The tag to unsubscribe from.
 */
void RTreeObject::removeTag(int tag){
    tags.erase(tag);
}

/**
 * Determines whether the object is subscribed to the given tag.
 *
 * @param tag The tag to check against.
 * @return true if this object subscribes to the given tag, and false otherwise.
 */
bool RTreeObject::containsTag(int tag){
    return tag == -1 || tags.find(tag) != tags.end();
}

/**
 * Determines whether the object is subscribed to any tags.
 *
 * @return true if this object is subscribed to at least one tag, and false otherwise.
 */
bool RTreeObject::subscribesToTag(){
    return !tags.empty();
}
