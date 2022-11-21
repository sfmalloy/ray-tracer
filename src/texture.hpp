#pragma once

#include "types.hpp"

class texture {
public:
    virtual color value(f32 u, f32 v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color();
    solid_color(const color& c);
    solid_color(f32 r, f32 g, f32 b);
    color value(f32 u, f32 v, const point3& p) const override;
private:
    color m_color;
};
