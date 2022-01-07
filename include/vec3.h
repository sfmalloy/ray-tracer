#pragma once

#include <ostream>

#include "types.h"
#include "utils.h"

class vec3 {
public:
    // Constructors
    vec3();
    vec3(f32 x, f32 y, f32 z);

    // Getters
    f32 x() const;
    f32 y() const;
    f32 z() const;

    // Mutable operators
    vec3& operator+=(const vec3& other);
    vec3& operator-=(const vec3& other);
    vec3& operator*=(const f32 t);
    vec3& operator/=(const f32 t);

    // Other operations
    f32 length() const;
    f32 length_squared() const;
    bool near_zero() const;

private:
    f32 m_x;
    f32 m_y;
    f32 m_z;
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    out << "[ " << v.x() << ' ' << v.y() << ' ' << v.z() << " ]";
    return out;
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return static_cast<vec3>(u) += v;
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return static_cast<vec3>(u) -= v;
}

inline vec3 operator*(const vec3& u, const vec3&v) {
    return vec3{u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
}

inline vec3 operator*(f32 t, const vec3& v) {
    return static_cast<vec3>(v) *= t;
}

inline vec3 operator*(const vec3& v, f32 t) {
    return static_cast<vec3>(v) *= t;
}

inline vec3 operator-(const vec3& u) {
    return static_cast<vec3>(u) * -1.0;
}

inline vec3 operator/(const vec3& v, f32 t) {
    return static_cast<vec3>(v) /= t;
}

inline f32 dot(const vec3& u, const vec3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3 {
        u.y() * v.z() - u.z() * v.y(),
        u.z() * v.x() - u.x() * v.z(),
        u.x() * v.y() - u.y() * v.x()
    };
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 randvec() {
    return vec3{randf32(), randf32(), randf32()};
}

inline vec3 randvec(f32 min, f32 max) {
    return vec3{randf32(min, max), randf32(min, max), randf32(min, max)};
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

using point3 = vec3;

vec3 refract(const vec3& uv, const vec3& normal, f32 etai_over_etat);

vec3 randvec_unit_sphere();

vec3 randvec_hemisphere(const vec3& normal);

vec3 randvec_unit_disk();
