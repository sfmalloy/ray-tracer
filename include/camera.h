#pragma once

#include "types.h"
#include "vec3.h"
#include "ray.h"

class camera {
public:
    camera(
        const point3& lookfrom, 
        const point3& lookat, 
        const vec3& up, 
        f32 vertical_fov, 
        f32 aspect_ratio,
        f32 aperture,
        f32 focus_dist
    );

    ray get_ray(f32 u, f32 v) const;

private:
    point3 m_origin;
    point3 m_lower_left_corner;
    vec3 m_horizontal;
    vec3 m_vertical;
    vec3 u, v, w;
    f32 lens_radius;
};
