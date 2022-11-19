#pragma once

#include <memory>
#include <vector>

#include "aabb.hpp"
#include "types.hpp"
#include "hittable.hpp"
#include "ray.hpp"

class hittable_list : public hittable {
public:
    hittable_list();
    hittable_list(std::shared_ptr<hittable> object);

    void clear();
    void add(std::shared_ptr<hittable> object);

    virtual bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;
    virtual bool bounding_box(f32 t0, f32 t1, aabb& output_box) const override;

    size_t size() const {
        return m_objects.size();
    }

public:
    std::vector<std::shared_ptr<hittable>> m_objects;
};
