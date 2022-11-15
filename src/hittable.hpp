#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "aabb.hpp"
#include "types.hpp"
#include "ray.hpp"
#include "material.hpp"

class material;

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    std::shared_ptr<material> mat_ptr;
    f32 t;
    bool front_face;

    inline void set_face_normal(const ray& r, const glm::vec3& outward_normal) {
        front_face = glm::dot(r.direction(), outward_normal) < 0.0f;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(f32 t0, f32 t1, aabb& output_box) const = 0;
};
