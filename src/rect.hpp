#pragma once

#include <glm/glm.hpp>
#include "types.hpp"
#include "hittable.hpp"

class rect : public hittable {
public:
    rect();
    rect(const point3& a, const point3& b, const glm::vec3& normal, std::shared_ptr<material> mat);
    bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;

private:
    point3 m_min;
    point3 m_max;
    f32 m_k;
    glm::vec3 m_normal;
    std::shared_ptr<material> m_mat;
};
