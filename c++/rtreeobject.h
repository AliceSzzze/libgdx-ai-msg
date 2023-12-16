#ifndef OBJ_H
#define OBJ_H

#include <memory>
#include <random>
#include <string>
#include <vector>
#include <unordered_set>
#include <cugl/cugl.h>

using namespace cugl;

/**
 * Container class for objects stored in the RTree that specifies its bounding box.
 *
 * The object can optionally have a set of integer tags, which allows it to be matched
 * with search queries for that given tag.
 */
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
