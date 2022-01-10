#include "color.hpp"
#include "material.hpp"
#include "vec3_utils.hpp"
#include "utils.hpp"

lambertian::lambertian(const color& albedo)
  : m_albedo{albedo}
{ }

bool lambertian::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    // glm::vec3 dir = rec.normal + randvec();
    glm::vec3 dir = rec.p + randvec_hemisphere(rec.normal);
    if (vec3_near_zero(dir))
        dir = rec.normal;

    scattered = ray{rec.p, dir - rec.p};
    attenuation = m_albedo;
    return true;
}

metal::metal(const color& albedo, f32 fuzz)
  : m_albedo{albedo},
    m_fuzz{fuzz < 1.0f ? fuzz : 1.0f}
{ }

bool metal::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
    scattered = ray{rec.p, reflected + m_fuzz * randvec_unit_sphere()};
    attenuation = m_albedo;

    return dot(scattered.direction(), rec.normal) > 0;
}

dielectric::dielectric(f32 index_of_refraction)
  : m_ir{index_of_refraction}
{ }

bool dielectric::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    attenuation = color{1.0f, 1.0f, 1.0f};
    f32 refraction_ratio = rec.front_face ? (1.0f / m_ir) : m_ir;

    glm::vec3 unit_dir = glm::normalize(r_in.direction());
    f32 cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.0f);
    f32 sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
    glm::vec3 dir;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randf32())
        dir = glm::reflect(unit_dir, rec.normal);
    else
        dir = refract(unit_dir, rec.normal, refraction_ratio);

    scattered = ray{rec.p, dir};
    return true;
}
