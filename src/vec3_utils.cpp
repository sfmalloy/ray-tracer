#include <cmath>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "vec3_utils.hpp"
#include "utils.hpp"

f32 vec3_length_squared(const glm::vec3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

bool vec3_near_zero(const glm::vec3& v) {
    return std::fabs(v.x) < EPS && std::fabs(v.y) < EPS && std::fabs(v.z) < EPS;
}

glm::vec3 refract(const glm::vec3& uv, const glm::vec3& normal, f32 etai_over_etat) {
    f32 cosine = std::fmin(glm::dot(-uv, normal), 1.0f);
    glm::vec3 r_out_perp = etai_over_etat * (uv + cosine * normal);
    glm::vec3 r_out_parallel = -std::sqrt(std::fabs(1.0f - vec3_length_squared(r_out_perp))) * normal;
    return r_out_perp + r_out_parallel;
}

glm::vec3 randvec() {
    return glm::vec3{randf32(), randf32(), randf32()};
}

glm::vec3 randvec(f32 min, f32 max) {
    return glm::vec3{randf32(min, max), randf32(min, max), randf32(min, max)};
}

glm::vec3 randvec_unit_sphere() {
    glm::vec3 p;
    do {
        p = randvec(-1.0f, 1.0f);
    } while (vec3_length_squared(p) >= 1.0f);
    return p;
}

glm::vec3 randvec_hemisphere(const glm::vec3& normal) {
    glm::vec3 vec_unit_sphere = randvec_unit_sphere();
    if (glm::dot(vec_unit_sphere, normal) < 0.0f)
        return -vec_unit_sphere;
    return vec_unit_sphere;
}

glm::vec3 randvec_unit_disk() {
    glm::vec3 p;
    do {
        p = glm::vec3{randf32(-1.0f, 1.0f), randf32(-1.0f, 1.0f), 0.0f};
    } while (vec3_length_squared(p) >= 1.0f);
    return p;
}
