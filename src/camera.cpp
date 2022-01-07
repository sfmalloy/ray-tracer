#include "camera.h"
#include "utils.h"
#include "vec3.h"

camera::camera(
        const point3& lookfrom, 
        const point3& lookat, 
        const vec3& up, 
        f32 vertical_fov, 
        f32 aspect_ratio,
        f32 aperture,
        f32 focus_dist
) {
    f32 theta = degrees_to_radians(vertical_fov);
    f32 h = std::tan(theta / 2);
    f32 viewport_height = 2.0 * h;
    f32 viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(up, w));
    v = cross(w, u);

    m_origin = lookfrom;
    m_horizontal = focus_dist * viewport_width * u;
    m_vertical = focus_dist * viewport_height * v;
    m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2.0;
}

ray camera::get_ray(f32 s, f32 t) const {
    vec3 radius = lens_radius * randvec_unit_disk();
    vec3 offset = u * radius.x() + v * radius.y();

    return ray {
        m_origin + offset, 
        m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset
    };
}
