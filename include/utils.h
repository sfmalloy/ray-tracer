#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#include "types.h"

const f32 INF = std::numeric_limits<f32>::infinity();
const f32 PI  = 3.1415926535897932385;

inline f32 degrees_to_radians(f32 degrees) {
    return degrees * PI / 180.0;
}

inline f32 randf32() {
    return std::rand() / (static_cast<f32>(RAND_MAX) + 1.0);
}

inline f32 randf32(f32 min, f32 max) {
    return min + (max - min) * randf32();
}
