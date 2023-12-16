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

#ifndef OBJ_H
#define OBJ_H

#include <memory>
#include <random>
#include <string>
#include <vector>
#include <unordered_set>
#include <cugl/cugl.h>

using namespace cugl;

class RTreeObject {
private:
    /** The tags that this object subscribes to. */
    std::unordered_set<int> tags;
public:
    /** The bounding box of this object. */
    Rect rect;

    /**
    * Creates a new RTreeObject that doesn't subscribe to any tags.
    *
    * @param x The x-coordinate of the lower-left corner.
    * @param y The y-coordinate of the lower-left corner.
    * @param width The x-coordinate of the upper-right corner.
    * @param height The y-coordinate of the upper-right corner.
    */
    RTreeObject(float x, float y, float width, float height);
    
    /**
     * Adds a tag to the set of subscribed tags.
     *
     * @param tag The tag to subscribe to.
     */
    void addTag(int tag);
    
    /**
     * Removes a tag from the set of subscribed tags.
     *
     * @param tag The tag to unsubscribe from.
     */
    void removeTag(int tag);
    
    /**
     * Determines whether the object is subscribed to the given tag. Always true if tag is -1.
     *
     * @param tag The tag to check against.
     * @return true if this object subscribes to the given tag, and false otherwise.
     */
    bool containsTag(int tag);
    
    /**
     * Determines whether the object is subscribed to any tags.
     *
     * @return true if this object is subscribed to at least one tag, and false otherwise.
     */
    bool subscribesToTag();
    
    /**
     * Deconstruct this RTreeObject.
     */
    virtual ~RTreeObject(){
        
    }
};

#endif
