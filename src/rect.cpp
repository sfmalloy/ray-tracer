#include "rect.hpp"

rect::rect()
{ }

rect::rect(const point3& a, const point3& b, const glm::vec3& normal, std::shared_ptr<material> mat)
  : m_min{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)}
  , m_max{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)}
  , m_normal{normal}
  , m_k{normal.x * a.x + normal.y * a.y + normal.z * a.z}
  , m_mat{mat}
{ }

bool rect::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    f32 t = (m_k - glm::dot(m_normal, r.origin())) / glm::dot(m_normal, r.direction());
    if (t < t_min || t > t_max)
        return false;

    auto pt = r.at(t);
    if (pt.x < m_min.x || pt.y < m_min.y || pt.z < m_min.z 
        || pt.x > m_max.x || pt.y > m_max.y || pt.z > m_max.z) {
        return false;
    }

    rec.normal = m_normal;
    rec.t = t;
    rec.p = pt;
    rec.set_face_normal(r, m_normal);
    rec.mat_ptr = m_mat;

    return true;
}
