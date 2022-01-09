#pragma once

#include <memory>

#include "types.h"
#include "material.h"

class sphere : public hittable {
public:
    sphere();
    sphere(point3 center, f32 radius, std::shared_ptr<material> m);
    bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;

private:
    point3 m_center;
    f32 m_radius;
    std::shared_ptr<material> m_mat_ptr;
};
