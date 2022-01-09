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

/*****************************************************************************/
// Local includes
#include "color.h"
#include "utils.h"
#include "ray.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "vec3_utils.h"

/*****************************************************************************/
// Forward decls
color ray_color(const ray& r, const hittable& world, u32 depth);

hittable_list random_scene();

std::string process_rows(u32 tid, i32 start, i32 end, i32 img_height, i32 img_width, i32 samples, 
                         const camera& cam, const hittable_list& world, const i32 max_depth);

std::ostream& operator<<(std::ostream& out, const glm::vec3& v) {
    out << "[ " << v.x << ' ' << v.y << ' ' << v.z << " ]";
    return out;
}

/*****************************************************************************/
// Inline util functions
inline u32 partition(u32 tid, u32 p, u32 n) {
    return n * tid / p;
}

/*****************************************************************************/
// Globals
std::vector<i32> g_counts;

/*****************************************************************************/

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Please enter filename\nUsage: ./RayTracer <filename>.ppm <thread_count>\n";
        return -1;
    }

    /*********************************************************************/
    // Change this stuff to change the render

    // render settings
    const f32 aspect_ratio = 16.0f / 9.0f;
    const i32 img_width = 400;
    const i32 img_height = static_cast<i32>(img_width / aspect_ratio);
    const u32 samples = 100;
    const u32 max_depth = 50;

    // camera settings
    glm::vec3 lookfrom{13.0f, 2.0f, 3.0f};
    glm::vec3 lookat{0.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    f32 vertical_fov = 20.0f;
    f32 dist_to_focus = 10.0f;
    f32 aperture = 0.1f;

    hittable_list world = random_scene();

    // end of changeable area
    /*********************************************************************/

    camera cam{lookfrom, lookat, up, vertical_fov, aspect_ratio, aperture, dist_to_focus};

    std::ofstream file{argv[1]};
    file << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    // hide terminal cursor
    std::printf("\e[?25l");

    u32 thread_count = std::stoi(std::string{argv[2]});
    if (thread_count == 1) {
        for (i32 row = img_height - 1; row >= 0; --row) {
            std::cerr << "\rScanlines remaining: " << row << ' ' << std::flush;
            for (i32 col = 0; col < img_width; ++col) {
                color pixel{};
                for (u32 s = 0; s < samples; ++s) {
                    f32 u = (col + randf32()) / (img_width - 1);
                    f32 v = (row + randf32()) / (img_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel += ray_color(r, world, max_depth);
                }
                write_color(file, pixel, samples);
            }
            file << '\n';
        }
    } else {
        std::vector<std::future<std::string>> futures;
        g_counts = std::vector<i32>(thread_count);
        for (u32 tid = 0; tid < thread_count; ++tid) {
            u32 start = partition(tid, thread_count, img_height);
            u32 end = partition(tid + 1, thread_count, img_height);
            futures.push_back(std::async(process_rows, tid, start, end, img_height, img_width, samples, std::cref(cam), std::cref(world), max_depth));
        }

        i32 sum;
        do {
            sum = std::accumulate(std::begin(g_counts), std::end(g_counts), 0);
            std::cerr << "\rScanlines finished: " << sum << "/" << img_height << ' ';
        } while (sum < img_height);

        for (auto it = std::rbegin(futures); it != std::rend(futures); ++it) {
            file << it->get();
        }
    }

    // re-show terminal cursor
    std::printf("\e[?25h");
    
    std::cerr << "\nDone\n";
    file.close();

    return 0;
}

/*****************************************************************************/

color ray_color(const ray& r, const hittable& world, u32 depth) {
    if (depth <= 0)
        return color{};
    
    hit_record rec;
    if (world.hit(r, 0.001f, INF, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color{};
    }

    // gradient sky
    glm::vec3 dir = glm::normalize(r.direction());
    f32 t = 0.5f * (dir.y + 1.0f);
    return (1.0f - t) * color{1.0f, 1.0f, 1.0f} + t * color{0.5f, 0.7f, 1.0f};
}

/*****************************************************************************/

hittable_list random_scene() {
    hittable_list world;

    auto ground = std::make_shared<lambertian>(color{0.5f, 0.5f, 0.5f});
    world.add(std::make_shared<sphere>(glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f, ground));

    glm::vec3 pt{4.0f, 0.2f, 0.0f};
    for (i32 i = -11; i < 11; ++i) {
        for (i32 j = -11; j < 11; ++j) {
            auto rand_mat = randf32();
            glm::vec3 center{i + 0.9f * randf32(), 0.2f, j + 0.9f * randf32()};

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

    world.add(std::make_shared<sphere>(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f, glass_mat));
    world.add(std::make_shared<sphere>(glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f, flat_mat));
    world.add(std::make_shared<sphere>(glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f, metal_mat));

    return world;
}

/*****************************************************************************/

std::string process_rows(u32 tid, i32 start, i32 end, i32 img_height, i32 img_width, i32 samples, 
                         const camera& cam, const hittable_list& world, const i32 max_depth) {
    std::string pixels;
    
    for (i32 row = end - 1; row >= start; --row) {
        for (i32 col = 0; col < img_width; ++col) {
            color pixel{};
            for (u32 s = 0; s < samples; ++s) {
                f32 u = (col + randf32()) / (img_width - 1);
                f32 v = (row + randf32()) / (img_height - 1);
                ray r = cam.get_ray(u, v);
                pixel += ray_color(r, world, max_depth);
            }
            pixels += color_string(pixel, samples);
        }
        pixels += '\n';
        ++g_counts[tid];
    }

    return pixels;
}
