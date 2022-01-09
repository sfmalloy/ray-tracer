#pragma once

#include <glm/vec3.hpp>

#include "ray.h"
#include "types.h"

class ray {
public:
    ray();
    ray(const glm::vec3& origin, const glm::vec3& direction);

    glm::vec3 origin() const;
    glm::vec3 direction() const;

    glm::vec3 at(f32 t) const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};
