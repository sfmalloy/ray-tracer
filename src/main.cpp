/*****************************************************************************/
// System includes
#include <iostream>
#include <cstdio>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <cmath>
#include <future>
#include <vector>
#include <numeric>
#include <iomanip>

/*****************************************************************************/
// Lib includes
#include <glm/vec3.hpp>
#include <yaml-cpp/node/parse.h>

/*****************************************************************************/
// Local includes
#include "color.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "vec3_utils.hpp"
#include "image.hpp"
#include "scene_loader.hpp"
#include "timer.hpp"
#include "scene_attributes.hpp"

/*****************************************************************************/
// Forward decls
color ray_color(const ray& r, const hittable& world, u32 depth);

hittable_list random_scene();

std::vector<u8color> process_rows(u32 tid, i32 start, i32 end, const scene_attributes& scene);

std::ostream& operator<<(std::ostream& out, const glm::vec3& v) {
    out << "[ " << v.x << ' ' << v.y << ' ' << v.z << " ]";
    return out;
}

/*****************************************************************************/
// Globals
std::vector<i32> g_counts;

/*****************************************************************************/

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Please enter filename\nUsage: ./RayTracer <filename>.ppm <thread_count> [<scene>.yml]\n";
        return -1;
    }
    std::srand(std::time(0));

    hittable_list world;
    if (argc >= 4) {
        auto node = YAML::LoadFile(argv[3]);
        world = node["scene"].as<hittable_list>();
    } else {
        world = random_scene();
    }

    /*********************************************************************/
    // Change this stuff to change the render

    // render settings
    const f32 aspect_ratio = 3.0f / 2.0f;
    const i32 img_width = 1280;
    const i32 img_height = static_cast<i32>(img_width / aspect_ratio);
    const u32 samples = 10;
    const u32 max_depth = 50;
    
    // camera settings
    glm::vec3 lookfrom{13.0f, 2.0f, 3.0f};
    glm::vec3 lookat{0.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    f32 vertical_fov = 20.0f;
    f32 dist_to_focus = 10.0f;
    f32 aperture = 0.1;

    // end of changeable area
    /*********************************************************************/

    camera cam{lookfrom, lookat, up, vertical_fov, aspect_ratio, aperture, dist_to_focus};
    timer t;
    scene_attributes scene{img_height, img_width, samples, max_depth, cam, world};

    // hide terminal cursor
    std::printf("\e[?25l");

    t.start();
    u32 thread_count = std::stoi(std::string{argv[2]});
    row_renderer row_render(thread_count);
    auto pixel_data = row_render.render(scene);
    if (!write_to_png(argv[1], pixel_data, scene.img_width, scene.img_height))
        std::cerr << "\nWrite fail";
    t.stop();

    // re-show terminal cursor
    std::printf("\e[?25h");
    std::cerr << "\nTime: " << std::fixed << std::setprecision(3) << t.elapsed_seconds() << "s\n";

    return 0;
}

/*****************************************************************************/

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
