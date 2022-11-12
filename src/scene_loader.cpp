#include "scene_loader.hpp"
#include "utils.hpp"
#include "vec3_utils.hpp"

hittable_list random_scene() {
    hittable_list world;

    auto ground = std::make_shared<lambertian>(color{0.5f, 0.5f, 0.5f});
    world.add(std::make_shared<sphere>(point3{0.0f, -1000.0f, 0.0f}, 1000.0f, ground));

    point3 pt{4.0f, 0.2f, 0.0f};
    for (i32 i = -11; i < 11; ++i) {
        for (i32 j = -11; j < 11; ++j) {
            f32 rand_mat = randf32();
            point3 center{i + 0.9f * randf32(), 0.2f, j + 0.9f * randf32()};

            if ((center - pt).length() > 0.9f) {
                std::shared_ptr<material> sphere_mat;

                if (rand_mat < 0.8f) {
                    // diffuse
                    color c1 = randvec();
                    color c2 = randvec();
                    sphere_mat = std::make_shared<lambertian>(c1 * c2);
                } else if (rand_mat < 0.95f) {
                    // metal
                    color albedo = randvec(0.5f, 1.0f);
                    f32 fuzz = randf32(0.0f, 0.5f);
                    sphere_mat = std::make_shared<metal>(albedo, fuzz);
                } else {
                    // glass
                    sphere_mat = std::make_shared<dielectric>(1.5f);
                }
                world.add(std::make_shared<sphere>(center, 0.2f, sphere_mat));
            }
        }
    }

    auto glass_mat = std::make_shared<dielectric>(1.5f);
    auto flat_mat = std::make_shared<lambertian>(color{0.4f, 0.2f, 0.1f});
    auto metal_mat = std::make_shared<metal>(color{0.7f, 0.6f, 0.5f}, 0.0f);

    world.add(std::make_shared<sphere>(point3{0.0f, 1.0f, 0.0f}, 1.0f, glass_mat));
    world.add(std::make_shared<sphere>(point3{-4.0f, 1.0f, 0.0f}, 1.0f, flat_mat));
    world.add(std::make_shared<sphere>(point3{4.0f, 1.0f, 0.0f}, 1.0f, metal_mat));

    return world;
}
