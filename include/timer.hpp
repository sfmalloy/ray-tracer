#pragma once

#include <chrono>
#include <ratio>

class timer {
    using clock = std::chrono::system_clock;
    using time_point = clock::time_point;
    using duration = std::chrono::duration<double>;

public:
    timer() { }

    void start();
    void stop();
    double elapsed_seconds();
private:
    time_point m_start;
    time_point m_stop;
};
