#pragma once

#include <glm/vec3.hpp>

#include "types.h"
#include "ray.h"

class camera {
public:
    camera(
        const glm::vec3& lookfrom, 
        const glm::vec3& lookat, 
        const glm::vec3& up, 
        f32 vertical_fov, 
        f32 aspect_ratio,
        f32 aperture,
        f32 focus_dist
    );

    ray get_ray(f32 u, f32 v) const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_lower_left_corner;
    glm::vec3 m_horizontal;
    glm::vec3 m_vertical;
    glm::vec3 u, v, w;
    f32 lens_radius;
};
