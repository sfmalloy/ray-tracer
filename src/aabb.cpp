#include "aabb.hpp"
#include "ray.hpp"
#include "types.hpp"
#include <cmath>

aabb::aabb()
  : m_min{}
  , m_max{}
{ }

aabb::aabb(const point3& a, const point3& b)
  : m_min{a}
  , m_max{b}
{ }

point3 aabb::min() const {
    return m_min;
}

point3 aabb::max() const {
    return m_max;
}

bool aabb::hit(const ray &r, f32 t_min, f32 t_max) const {
    for (u32 i = 0; i < 3; ++i) {
        f32 inverse_dir = 1.0f / r.direction()[i];
        f32 t0 = (m_min[i] - r.origin()[i]) * inverse_dir;
        f32 t1 = (m_max[i] - r.origin()[i]) * inverse_dir;
        if (inverse_dir < 0.0f)
            std::swap(t0, t1);
        // f32 t0 = std::fmin(min_comp, max_comp);
        // f32 t1 = std::fmax(min_comp, max_comp);

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        // preventing NaNs and stuff
        if (t_max <= t_min)
            return false;
    }
    return true;
}