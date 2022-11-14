#pragma once

#include "ray.hpp"
#include "types.hpp"

class aabb {
public:
    aabb();
    aabb(const point3& a, const point3& b);

    point3 min() const;
    point3 max() const;

    bool hit(const ray& r, f32 t_min, f32 t_max) const;
private:
    point3 m_min;
    point3 m_max;
};
