#pragma once

#include <glm/vec3.hpp>

#include "types.hpp"

f32 vec3_length_squared(const glm::vec3& v);

bool vec3_near_zero(const glm::vec3& v);

glm::vec3 refract(const glm::vec3& uv, const glm::vec3& normal, f32 etai_over_etat);

glm::vec3 randvec();

glm::vec3 randvec(f32 min, f32 max);

glm::vec3 randvec_unit_sphere();

glm::vec3 randvec_hemisphere(const glm::vec3& normal);

glm::vec3 randvec_unit_disk();
