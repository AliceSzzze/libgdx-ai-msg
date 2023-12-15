#include "rtreeobject.h"
#include <cugl/cugl.h>

using namespace cugl;

RTreeObject::RTreeObject(float x, float y, float width, float height) {
    rect = Rect(x, y, width, height);
    velX = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2));
    velY = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2));
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

void RTreeObject::update(float mapWidth, float mapHeight) {
    if (rect.getMaxX() + velX >= mapWidth || rect.getMinX() + velX < 0) {
        velX = -velX;
    }
    if (rect.getMaxY() + velY >= mapHeight || rect.getMinY() + velY < 0) {
        velY = -velY;
    }
    rect.origin.x += velX;
    rect.origin.y += velY;
}

std::string RTreeObject::print() {
    return "[(" + std::to_string(rect.getMinX()) + ", " +
           std::to_string(rect.getMinY()) + "), (" +
           std::to_string(rect.getMaxX()) + ", " +
           std::to_string(rect.getMaxY()) + ")]\n";
}

void RTreeObject::draw(const std::shared_ptr<SpriteBatch>& batch) {
    Rect r = Rect((rect.origin.x) / 1024, (rect.origin.y) / 576,
                  (rect.size.width) / 1024, (rect.size.height) / 576);
    batch->outline(r);
}
