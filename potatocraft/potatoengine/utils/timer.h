#pragma once

#include <chrono>

namespace potatoengine {

class Timer {
   public:
    Timer() { reset(); }

    void reset() { m_start = std::chrono::high_resolution_clock::now(); }
    float elapsed() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_start).count() * 0.001f * 0.001f * 0.001f; }
    float elapsedMS() { return elapsed() * 1000.f; }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

}