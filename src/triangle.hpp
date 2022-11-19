#pragma once

#include <glm/vec3.hpp>
#include <memory>

#include "material.hpp"
#include "types.hpp"
#include "hittable.hpp"

class triangle : public hittable {
public:
    triangle();
    triangle(const point3& a, const point3& b, const point3& c, std::shared_ptr<material> mat);
    bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;

private:
    bool same_side(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const;

    point3 m_a;
    point3 m_b;
    point3 m_c;
    std::shared_ptr<material> m_mat_ptr;

    point3 m_min;
    point3 m_max;

    glm::vec3 m_T; // vector perp to triangle == ab x ac
    f32 m_k; // plane constant
};
