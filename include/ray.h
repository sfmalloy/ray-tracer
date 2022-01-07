#pragma once

#include "vec3.h"
#include "ray.h"

class ray {
public:
    ray();
    ray(const point3& origin, const vec3& direction);

    point3 origin() const;
    vec3 direction() const;

    point3 at(f32 t) const;

private:
    point3 m_origin;
    vec3 m_direction;
};
