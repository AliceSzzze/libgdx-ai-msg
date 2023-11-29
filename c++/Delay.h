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
    Delay(int measuredDelay, int expectedDelay) : measuredDelay(measuredDelay), expectedDelay(expectedDelay) {};
    uint64_t measuredDelay;
    uint64_t expectedDelay;
};


#endif //LIBGDX_DELAY_H
