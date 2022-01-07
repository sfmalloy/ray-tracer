#include "ray.h"
#include "vec3.h"

ray::ray() { }

ray::ray(const point3& origin, const vec3& direction) 
  : m_origin{origin},
    m_direction{direction}
{ }

point3 ray::origin() const {
    return m_origin;
}

point3 ray::direction() const {
    return m_direction;
}

point3 ray::at(f32 t) const {
    return m_origin + t * m_direction;
}
