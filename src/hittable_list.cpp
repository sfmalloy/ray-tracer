#include <iostream>

#include <memory>
#include <vector>

#include "types.hpp"
#include "ray.hpp"
#include "hittable_list.hpp"
#include "hittable.hpp"

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
