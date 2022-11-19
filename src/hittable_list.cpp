#include "hittable_list.hpp"
#include "aabb.hpp"

hittable_list::hittable_list() { }

hittable_list::hittable_list(std::shared_ptr<hittable> object) {
    add(object);
}

void hittable_list::clear() {
    m_objects.clear();
}

void hittable_list::add(std::shared_ptr<hittable> object) {
    m_objects.push_back(object);
}

bool hittable_list::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool has_hit = false;
    f32 t_closest = t_max;

    for (const auto& obj : m_objects) {
        if (obj->hit(r, t_min, t_closest, temp_rec)) {
            has_hit = true;
            t_closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return has_hit;
}

bool hittable_list::bounding_box(f32 t0, f32 t1, aabb& output_box) const {
    if (m_objects.empty())
        return false;
    
    aabb temp_box;
    bool first = true;

    for (const auto& obj : m_objects) {
        if (!obj->bounding_box(t0, t1, temp_box))
            return false;
        output_box = first ? temp_box : surrounding_box(output_box, temp_box);
        first = false;
    }

    return true;
}
