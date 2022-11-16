#include <glm/vec3.hpp>
#include <memory>
#include <fmt/core.h>

#include "triangle.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "vec3_utils.hpp"

triangle::triangle()
{ }

triangle::triangle(const point3& a, const point3& b, const point3& c, std::shared_ptr<material> mat)
  : m_a{a}
  , m_b{b}
  , m_c{c}
  , m_mat_ptr{mat}
  , m_T{glm::cross(b - a, c - a)}
  , m_k{m_T.x * m_a.x + m_T.y * m_a.y + m_T.z * m_a.z} // technically could do dot product but m_a is a 
                                                       // "point" so I won't
{ }

bool triangle::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    f32 t = (m_k - glm::dot(m_T, r.origin())) / glm::dot(m_T, r.direction());
    if (t < t_min || t > t_max)
        return false;

    auto p = r.at(t);
    if (!same_side(p, m_a, m_b, m_c) || !same_side(p, m_b, m_a, m_c) || !same_side(p, m_c, m_b, m_a)) {
        return false;
    }

    rec.t = t;
    rec.p = p;
    rec.set_face_normal(r, m_T);
    rec.mat_ptr = m_mat_ptr;

    return true;
}

bool triangle::same_side(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const {
    auto pcross = glm::cross(b - a, p - a);
    auto ref = glm::cross(b - a, c - a);
    return glm::dot(pcross, ref) >= 0;
}
