#include <cmath>
#include <memory>

#include "vec3.h"
#include "sphere.h"

sphere::sphere()
{ }

sphere::sphere(point3 center, f32 radius, std::shared_ptr<material> mat)
  : m_center{center},
    m_radius{radius},
    m_mat_ptr{mat}
{ }

bool sphere::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    vec3 oc = r.origin() - m_center;
    f32 a = r.direction().length_squared();
    f32 half_b = dot(oc, r.direction());
    f32 c = oc.length_squared() - m_radius * m_radius;

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
    vec3 outward_normal = (rec.p - m_center) / m_radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = m_mat_ptr;

    return true;
}
