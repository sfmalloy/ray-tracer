#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>
#include <cmath>

#include "types.hpp"
#include "color.hpp"

void write_color(std::ostream& out, const color& c, u32 samples) {
    f32 scale = 1.0 / samples;
    f32 r = std::sqrt(scale * c.r);
    f32 g = std::sqrt(scale * c.g);
    f32 b = std::sqrt(scale * c.b);

    out << static_cast<i32>(256 * std::clamp(r, 0.0f, 0.999f)) << ' '
        << static_cast<i32>(256 * std::clamp(g, 0.0f, 0.999f)) << ' '
        << static_cast<i32>(256 * std::clamp(b, 0.0f, 0.999f)) << '\n';
}

void write_color_gray(std::ostream& out, const color& c, u32 samples) {
    f32 scale = 1.0 / samples;
    f32 r = std::sqrt(scale * c.r);
    f32 g = std::sqrt(scale * c.g);
    f32 b = std::sqrt(scale * c.b);

    i32 gray = static_cast<i32>(256 * std::clamp((r + g + b) / 3.0, 0.0, 0.999));
    out << gray << ' ' << gray << ' ' << gray << '\n';
}

std::string color_string(const color& c, u32 samples) {
    std::string cstr;

    f32 scale = 1.0f / samples;
    f32 r = std::sqrt(scale * c.r);
    f32 g = std::sqrt(scale * c.g);
    f32 b = std::sqrt(scale * c.b);

    cstr  = std::to_string(static_cast<i32>(256 * std::clamp(r, 0.0f, 0.999f))) + ' '
          + std::to_string(static_cast<i32>(256 * std::clamp(g, 0.0f, 0.999f))) + ' '
          + std::to_string(static_cast<i32>(256 * std::clamp(b, 0.0f, 0.999f))) + '\n';

    return cstr;
}

u8color to_u8color(const color& c, u32 samples) {
    f32 scale = 1.0f / samples;
    f32 r = std::sqrt(scale * c.r);
    f32 g = std::sqrt(scale * c.g);
    f32 b = std::sqrt(scale * c.b);

    u8 u_r = static_cast<u8>(256 * std::clamp(r, 0.0f, 0.999f));
    u8 u_g = static_cast<u8>(256 * std::clamp(g, 0.0f, 0.999f));
    u8 u_b = static_cast<u8>(256 * std::clamp(b, 0.0f, 0.999f));

    return u8color{u_r, u_g, u_b};
}
