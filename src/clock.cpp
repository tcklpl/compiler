#include "clock.h"

Clock::Clock() {
    start = std::chrono::high_resolution_clock::now();
}

void Clock::stop() {
    end = std::chrono::high_resolution_clock::now();
    stopped = true;
}

double Clock::get_ms() {
    const std::chrono::duration<double, std::milli> ms = end - start;
    return ms.count();
}