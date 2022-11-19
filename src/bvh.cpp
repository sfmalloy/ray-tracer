#include <algorithm>
#include <iterator>
#include <memory>

#include <fmt/core.h>

#include "aabb.hpp"
#include "bvh.hpp"
#include "hittable_list.hpp"
#include "utils.hpp"
#include "vec3_utils.hpp"

using comp_func = bool(*)(const std::shared_ptr<hittable>, const std::shared_ptr<hittable>);

bool bvh_node::bounding_box(f32 t0, f32 t1, aabb& output_box) const {
    output_box = m_box;
    return true;
}

bool bvh_node::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    if (!m_box.hit(r, t_min, t_max))
        return false;

    bool left = m_left->hit(r, t_min, t_max, rec);
    bool right = m_right->hit(r, t_min, left ? rec.t : t_max, rec);

    return left || right;
}

bvh_node::bvh_node(const hittable_list& list, f32 t0, f32 t1)
  : bvh_node{list.m_objects, 0, list.m_objects.size(), t0, t1}
{ }

bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, f32 t0, f32 t1
) {
    i32 axis = randi32(0, 2);
    comp_func comp = (axis == 0) ? compare_x
                   : (axis == 1) ? compare_y
                   : compare_z;

    auto objects = src_objects;
    auto span = end - start;
    if (span == 1) {
        m_left = m_right = objects[start];
    } else if (span == 2) {
        if (comp(objects[start], objects[start + 1])) {
            m_left = objects[start];
            m_right = objects[start + 1];
        } else {
            m_left = objects[start + 1];
            m_right = objects[start];
        }
    } else {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comp);
        auto mid = start + span / 2;
        m_left = std::make_shared<bvh_node>(objects, start, mid, t0, t1);
        m_right = std::make_shared<bvh_node>(objects, mid, end, t0, t1);
    }

    aabb box_left;
    aabb box_right;
    if (!m_left->bounding_box(t0, t1, box_left) || !m_right->bounding_box(t0, t1, box_right)) {
        fmt::print("No bounding box in bvh_node ctor\n");
    }

    m_box =surrounding_box(box_left, box_right);
}
