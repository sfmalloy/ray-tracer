#pragma once

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

#include "triangle.hpp"
#include "hittable_list.hpp"

// not necessarily a proper mesh class but it'll do for now since this isn't realtime
class mesh : public hittable {
public:
    mesh();
    mesh(const std::string& filename, std::shared_ptr<material> mat, f32 scale);
    mesh(const std::string& filename, std::shared_ptr<material> mat, const glm::vec3& offset, f32 scale);
    bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;
private:
    hittable_list m_faces;
};
