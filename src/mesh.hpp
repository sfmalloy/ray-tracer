#include "triangle.hpp"
#include "hittable_list.hpp"

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

// not necessarily a proper mesh class but it'll do for now since this isn't realtime
class mesh : public hittable {
public:
    mesh();
    mesh(const std::string& filename, std::shared_ptr<material> mat);
    mesh(const std::string& filename, std::shared_ptr<material> mat, const glm::vec3& offset);
    bool hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const override;
private:
    hittable_list m_faces;
};
