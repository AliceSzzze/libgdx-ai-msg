#include "Telegraph.h"

// Used for testing/visualization purposes. Should be removed before it's added to CUGL
class TestObject : public Telegraph {
public:
    float velX;
    float velY;
    bool wasAlerted;
    TestObject(float x, float y, float width, float height)
    : Telegraph(x, y, width, height) {
        velX = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2));
        velY = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2));
    };
    
    TestObject(float x, float y, float width, float height, float radius)
    : Telegraph(x, y, width, height, radius) {
        velX = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2));
        velY = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2));
    };
    
    void handleMessage(const std::shared_ptr<Telegram> msg) {
        wasAlerted = true;
    }
    
    void update(float mapWidth, float mapHeight){
        if (rect.getMaxX() + velX >= mapWidth || rect.getMinX() + velX < 0) {
            velX = -velX;
        }
        if (rect.getMaxY() + velY >= mapHeight || rect.getMinY() + velY < 0) {
            velY = -velY;
        }
        rect.origin.x += velX;
        rect.origin.y += velY;
    }
    
    void draw(const std::shared_ptr<SpriteBatch>& batch) {
        if(wasAlerted){
            batch->setColor(Color4::RED);
        }
        else{
            batch->setColor(Color4::BLACK);
        }
        
        Rect r = Rect((rect.origin.x) / 1024, (rect.origin.y) / 576,
                      (rect.size.width) / 1024, (rect.size.height) / 576);
        batch->outline(r);
        wasAlerted = false;
    }
};
