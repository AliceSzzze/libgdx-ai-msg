//
//  HelloApp.cpp
//  Cornell University Game Library (CUGL)
//
//  This is the implementation file for the custom application. This is the
//  definition of your root (and in this case only) class.
//
//  CUGL zlib License:
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
//  Author: Walker White
//  Version: 1/8/17
//
// Include the class header, which includes all of the CUGL classes
#include "HelloApp.h"
#include <cugl/base/CUBase.h>
#include "rtree.h"
#include "MessageDispatcher.h"
#include "TestObject.h"

// Add support for simple random number generation
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

// This keeps us from having to write cugl:: all the time
using namespace cugl;

// The number of frames before moving the logo to a new position
#define TIME_STEP 60
// This is adjusted by screen aspect ratio to get the height
#define GAME_WIDTH 1024

int numberOfObjects = 100;
int width = 5;
int height = 5;
auto start = std::chrono::steady_clock::now();
int toRemove = 0;

MessageDispatcher messageDispatcherInstance = MessageDispatcher(0, 0, 1, 1);
std::vector<std::shared_ptr<TestObject>> objects = {};

static void test_cases() {
    
}

/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
void HelloApp::onStartup() {
    Size size = getDisplaySize();
    size *= GAME_WIDTH/size.width;
    
    // Create a scene graph the same size as the window
    _scene = Scene2::alloc(size.width, size.height);
    // Create a sprite batch (and background color) to render the scene
    _batch = SpriteBatch::alloc();
    setClearColor(Color4(229,229,229,255));
    
    // Create an asset manager to load all assets
    _assets = AssetManager::alloc();
    
    // You have to attach the individual loaders for each asset type
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    
    // This reads the given JSON file and uses it to load all other assets
    _assets->loadDirectory("json/assets.json");

    // Activate mouse or touch screen input as appropriate
    // We have to do this BEFORE the scene, because the scene has a button
#if defined (CU_TOUCH_SCREEN)
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
#endif
    
    // Build the scene from these assets
    buildScene();
    Application::onStartup();
    
    // Report the safe area
    Rect bounds = Display::get()->getSafeBounds();
    CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
                            bounds.size.toString().c_str());

    bounds = getSafeBounds();
    CULog("Safe Area %sx%s",bounds.origin.toString().c_str(),
                            bounds.size.toString().c_str());

    bounds = getDisplayBounds();
    CULog("Full Area %sx%s",bounds.origin.toString().c_str(),
                            bounds.size.toString().c_str());

}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void HelloApp::onShutdown() {
    // Delete all smart pointers
    _logo = nullptr;
    _scene = nullptr;
    _batch = nullptr;
    _assets = nullptr;
    
    // Deativate input
#if defined CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
    Application::onShutdown();
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void HelloApp::update(float timestep) {
    Size size = getDisplaySize();
    
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        (*it)->update(size.width, size.height);
    }
    messageDispatcherInstance.update();
    messageDispatcherInstance.dispatchMessage(objects[10], 1);
    
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - start).count();

    if (elapsed == 2 && toRemove < objects.size()) {
        messageDispatcherInstance.removeListener(objects[toRemove], 1);
        messageDispatcherInstance.removeListener(objects[toRemove], 2);
        toRemove++;
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void HelloApp::draw() {
    // This takes care of begin/end
    _batch->begin();
    _batch->setColor(Color4::BLACK);
    messageDispatcherInstance.rtree->draw(_batch);
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        (*it)->draw(_batch);
    }
    _batch->end();
}

/**
 * Internal helper to build the scene graph.
 *
 * Scene graphs are not required.  You could manage all scenes just like
 * you do in 3152.  However, they greatly simplify scene management, and
 * have become standard in most game engines.
 */
void HelloApp::buildScene() {
    Size  size  = getDisplaySize();
    float scale = GAME_WIDTH/size.width;
    size *= scale;

    for (int i = 0; i < numberOfObjects; ++i) {
        float x1 = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / size.width));
        float y1 = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / size.height));
      auto obj = std::make_shared<TestObject>(x1, y1, width, height, 50);
        obj->addTag(1);
        obj->removeTag(1);

      objects.push_back(obj);
    }
    
    objects[10]->setSearchRadius(100);
    
    messageDispatcherInstance = MessageDispatcher(0, 0, size.width, size.height);
    messageDispatcherInstance.addMailbox(1);
    messageDispatcherInstance.addMailbox(2);
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        messageDispatcherInstance.addListener(*it, 1);
        messageDispatcherInstance.addListener(*it, 2);
    }
}
