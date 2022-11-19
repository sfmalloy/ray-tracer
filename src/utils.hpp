#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#include "types.hpp"

const constexpr f32 INF = std::numeric_limits<f32>::infinity();
const constexpr f32 PI  = 3.1415926535897932385;
const constexpr f32 EPS = 1e-8;

inline f32 degrees_to_radians(f32 degrees) {
    return degrees * PI / 180.0f;
}

inline f32 randf32() {
    return std::rand() / (static_cast<f32>(RAND_MAX) + 1.0f);
}

inline i32 randi32(i32 min, i32 max) {
    return min + std::rand() % ((max - min) + 1);
}

inline f32 randf32(f32 min, f32 max) {
    return min + (max - min) * randf32();
}

inline u32 partition(u32 tid, u32 p, u32 n) {
    return n * tid / p;
}

template <typename T>
T min(T a, T b, T c) {
    T m{std::numeric_limits<T>::max()};
    m = a < m ? a : m;
    m = b < m ? b : m;
    m = c < m ? c : m;
    return m;
}

template <typename T>
T max(T a, T b, T c) {
    T m{std::numeric_limits<T>::min()};
    m = a > m ? a : m;
    m = b > m ? b : m;
    m = c > m ? c : m;
    return m;
}
