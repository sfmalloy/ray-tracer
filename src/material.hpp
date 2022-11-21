#pragma once

#include <cmath>

#include "ray.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "texture.hpp"

struct hit_record;

class material {
public:
    virtual bool 
    scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;

    virtual color
    emitted(f32 u, f32 v, const point3& p) const;
};

class lambertian : public material {
public:
    lambertian(const color& albedo);

    virtual bool 
    scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
private:
    // color m_albedo;
    std::shared_ptr<solid_color> m_albedo;
};

class metal : public material {
public:
    metal(const color& albedo, f32 fuzz);

    virtual bool 
    scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
private:
    color m_albedo;
    f32 m_fuzz;
};

class dielectric : public material {
public:
    dielectric(f32 index_of_refraction);

    virtual bool 
    scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
private:
    f32 m_ir;

    static f32 reflectance(f32 cosine, f32 ref_idx) {
        f32 r = (1 - ref_idx) / (1 + ref_idx);
        r *= r;
        return r + (1 - r) * std::pow(1 - cosine, 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(std::shared_ptr<texture> a);
    diffuse_light(const color& c);

    virtual bool 
    scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

    virtual color
    emitted(f32 u, f32 v, const point3& p) const override;
private:
    std::shared_ptr<texture> m_emit;
};
