//
// Created by Alice Sze on 11/15/23.
//

#ifndef LIBGDX_DELAY_H
#define LIBGDX_DELAY_H

#include <ctime>
#include <chrono>
using namespace std::chrono;
class Delay {
public:
    Delay(Uint64 measuredDelay, Uint64 expectedDelay) : measuredDelayMicros(measuredDelay), expectedDelay(expectedDelay) {};
    Uint64 measuredDelayMicros;
    Uint64 expectedDelay;
};


#endif //LIBGDX_DELAY_H
