#include <ostream>
#include <string>
#include <algorithm>

#include "types.h"
#include "color.h"
#include "vec3.h"

color::color()
  : vec3()
{ }

color::color(const vec3& v)
  : color(v.x(), v.y(), v.z())
{ }

color::color(f32 r, f32 g, f32 b)
  : vec3(r, g, b)
{ }

f32 color::r() const {
    return x();
}

f32 color::g() const {
    return y();
}

f32 color::b() const {
    return z();
}

std::string color::get(u32 samples) const {
    std::string cstr;

    f32 scale = 1.0 / samples;
    f32 r = std::sqrt(scale * this->r());
    f32 g = std::sqrt(scale * this->g());
    f32 b = std::sqrt(scale * this->b());

    cstr  = std::to_string(static_cast<i32>(256 * std::clamp(r, 0.0f, 0.999f))) + ' '
          + std::to_string(static_cast<i32>(256 * std::clamp(g, 0.0f, 0.999f))) + ' '
          + std::to_string(static_cast<i32>(256 * std::clamp(b, 0.0f, 0.999f))) + '\n';
    
    return cstr;
}

void color::write(std::ostream& out, u32 samples) {
    f32 scale = 1.0 / samples;
    f32 r = std::sqrt(scale * this->r());
    f32 g = std::sqrt(scale * this->g());
    f32 b = std::sqrt(scale * this->b());

    out << static_cast<i32>(256 * std::clamp(r, 0.0f, 0.999f)) << ' '
        << static_cast<i32>(256 * std::clamp(g, 0.0f, 0.999f)) << ' '
        << static_cast<i32>(256 * std::clamp(b, 0.0f, 0.999f)) << '\n';
}

void color::write_gray(std::ostream& out, u32 samples) {
    f32 scale = 1.0 / samples;
    f32 r = std::sqrt(scale * this->r());
    f32 g = std::sqrt(scale * this->g());
    f32 b = std::sqrt(scale * this->b());

    i32 gray = static_cast<i32>(256 * std::clamp((r + g + b) / 3.0, 0.0, 0.999));
    out << gray << ' ' << gray << ' ' << gray << '\n';
}
