#include "timer.hpp"

void timer::start() {
    m_start = clock::now();
}

void timer::stop() {
    m_stop = clock::now();
}

double timer::elapsed_seconds() {
    duration d = m_stop - m_start;
    return d.count();
}
