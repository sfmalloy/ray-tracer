#include "texture.hpp"

solid_color::solid_color() { }

solid_color::solid_color(const color& c)
  : m_color{c}
{ }

solid_color::solid_color(f32 r, f32 g, f32 b)
  : m_color{r, g, b}
{ }

color solid_color::value(f32 u, f32 v, const point3& p) const {
    return m_color;
}
