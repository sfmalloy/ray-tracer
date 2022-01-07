#include "vec3.h"
#include "utils.h"

#include <cmath>

vec3::vec3()
  : m_x{0},
    m_y{0},
    m_z{0}
{ }

vec3::vec3(f32 x, f32 y, f32 z)
  : m_x{x},
    m_y{y},
    m_z{z}
{ }

f32 vec3::x() const {
    return m_x;
}

f32 vec3::y() const {
    return m_y;
}

f32 vec3::z() const {
    return m_z;
}

vec3& vec3::operator+=(const vec3& other) {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;

    return *this;
}

vec3& vec3::operator-=(const vec3& other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;

    return *this;
}

vec3& vec3::operator*=(const f32 t) {
    m_x *= t;
    m_y *= t;
    m_z *= t;
    
    return *this;
}

vec3& vec3::operator/=(const f32 t) {
    return *this *= 1 / t;
}

f32 vec3::length() const {
    return std::sqrt(length_squared());
}

f32 vec3::length_squared() const {
    return (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
}

bool vec3::near_zero() const {
    f32 eps = 1e-8;
    return std::fabs(m_x) < eps && std::fabs(m_y) < eps && std::fabs(m_z) < eps;
}

vec3 refract(const vec3& uv, const vec3& normal, f32 etai_over_etat) {
    f32 cosine = std::fmin(dot(-uv, normal), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cosine * normal);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_perp + r_out_parallel;
}

vec3 randvec_unit_sphere() {
    vec3 p;
    do {
        p = randvec(-1.0, 1.0);
    } while (p.length_squared() >= 1.0);
    return p;
}

vec3 randvec_hemisphere(const vec3& normal) {
    vec3 vec_unit_sphere = randvec_unit_sphere();
    if (dot(vec_unit_sphere, normal) < 0.0)
        return -vec_unit_sphere;
    return vec_unit_sphere;
}

vec3 randvec_unit_disk() {
    vec3 p;
    do {
        p = vec3{randf32(-1.0, 1.0), randf32(-1.0, 1.0), 0.0};
    } while (p.length_squared() >= 1.0);
    return p;
}
