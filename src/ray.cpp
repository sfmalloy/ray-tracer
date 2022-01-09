#include "ray.hpp"

ray::ray() { }

ray::ray(const glm::vec3& origin, const glm::vec3& direction) 
  : m_origin{origin},
    m_direction{direction}
{ }

glm::vec3 ray::origin() const {
    return m_origin;
}

glm::vec3 ray::direction() const {
    return m_direction;
}

glm::vec3 ray::at(f32 t) const {
    return m_origin + t * m_direction;
}
