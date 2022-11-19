#pragma once

#include <memory>
#include <vector>

#include <fmt/core.h>

#include "hittable.hpp"
#include "hittable_list.hpp"

class bvh_node : public hittable {
public:
    bvh_node();
    bvh_node(const hittable_list& list, f32 t0, f32 t1);
    bvh_node(
        const std::vector<std::shared_ptr<hittable>>& src_objects,
        size_t start,
        size_t end,
        f32 t0,
        f32 t1
    );

    bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;
    bool bounding_box(f32 t0, f32 t1, aabb& output_box) const override;

private:
    static bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, i32 axis) {
        aabb box_a;
        aabb box_b;

        if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
            fmt::print("No bounding box in bvh_node ctor");
        }

        return box_a.min()[axis] < box_b.min()[axis];
    }

    static bool compare_x(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool compare_y(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b){
        return box_compare(a, b, 1);
    }

    static bool compare_z(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b){
        return box_compare(a, b, 2);
    }

    std::shared_ptr<hittable> m_left;
    std::shared_ptr<hittable> m_right;
    aabb m_box;
};
