//
//  Delay.h
//
//  This class implements a Delay object, which is used to hold data about delays and
//   is purely for benchmarking. You could use a std::pair instead if you don't need more
//   than two pieces of information.
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

#ifndef CUGL_DELAY_H
#define CUGL_DELAY_H

#include <ctime>
#include <chrono>

using namespace std::chrono;
class Delay {
public:
    Delay(Uint64 measuredDelay, Uint64 expectedDelay) : measuredDelayMicros(measuredDelay), expectedDelay(expectedDelay) {};
    
    /// the unwanted delay (after subtracting the intended delay from the measured overall delay) in microseconds
    Uint64 measuredDelayMicros;
    
    /// the delay requested by the user in milliseconds
    Uint64 expectedDelay;
};


#endif //CUGL_DELAY_H
