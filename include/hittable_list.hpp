#pragma once

#include <memory>
#include <vector>

#include "types.hpp"
#include "hittable.hpp"

class hittable_list : public hittable {
public:
    hittable_list();
    hittable_list(std::shared_ptr<hittable> object);

    void clear();
    void add(std::shared_ptr<hittable> object);

    virtual bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;

    size_t size() const { return m_objects.size(); }

public:
    std::vector<std::shared_ptr<hittable>> m_objects;
};
