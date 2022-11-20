#include <cmath>
#include <memory>

#include "sphere.hpp"
#include "vec3_utils.hpp"
#include "utils.hpp"

sphere::sphere()
{ }

sphere::sphere(glm::vec3 center, f32 radius, std::shared_ptr<material> mat)
  : m_center{center},
    m_radius{radius},
    m_mat_ptr{mat}
{ }

bool sphere::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    glm::vec3 oc = r.origin() - m_center;
    f32 a = vec3_length_squared(r.direction());
    f32 half_b = dot(oc, r.direction());
    f32 c = vec3_length_squared(oc) - m_radius * m_radius;

    f32 discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    
    f32 sqrt_discriminant = std::sqrt(discriminant);

    // Find nearest root that lies in range
    f32 root = (-half_b - sqrt_discriminant) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root < t_min || t_max < root)
            return false;
    }


    rec.t = root;
    rec.p = r.at(rec.t);
    glm::vec3 outward_normal = (rec.p - m_center) / m_radius;
    auto uv = sphere::get_sphere_uv(outward_normal);
    rec.u = uv.x;
    rec.v = uv.y;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = m_mat_ptr;

    return true;
}

glm::vec2 sphere::get_sphere_uv(const point3 &p) {
    f32 theta = std::acos(-p.y);
    f32 phi = std::atan2(-p.z, p.x) + PI;

    return glm::vec2{phi / (2 * PI), theta / PI};
}
