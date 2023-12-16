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
