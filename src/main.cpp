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
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

/*****************************************************************************/
// Forward decls
color ray_color(const ray& r, const hittable& world, u32 depth);

hittable_list random_scene();

std::string process_rows(u32 tid, i32 start, i32 end, i32 img_height, i32 img_width, i32 samples, 
                         const camera& cam, const hittable_list& world, const i32 max_depth);

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
    const f32 aspect_ratio = 3.0 / 2.0;
    const i32 img_width = 1200;
    const i32 img_height = static_cast<i32>(img_width / aspect_ratio);
    const u32 samples = 500;
    const u32 max_depth = 50;

    // camera settings
    point3 lookfrom{13.0, 2.0, 3.0};
    point3 lookat{0.0, 0.0, 0.0};
    vec3 up{0.0, 1.0, 0.0};
    f32 vertical_fov = 20.0;
    f32 dist_to_focus = 10.0;
    f32 aperture = 0.1;

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
                color pixel;
                for (u32 s = 0; s < samples; ++s) {
                    f32 u = (col + randf32()) / (img_width - 1);
                    f32 v = (row + randf32()) / (img_height - 1);
                    ray r = cam.get_ray(u, v);
                    pixel += ray_color(r, world, max_depth);
                }
                pixel.write(file, samples);
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
    if (world.hit(r, 0.001, INF, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color{};
    }

    // gradient sky
    vec3 dir = unit_vector(r.direction());
    f32 t = 0.5 * (dir.y() + 1.0);
    return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
}

/*****************************************************************************/

hittable_list random_scene() {
    hittable_list world;

    auto ground = std::make_shared<lambertian>(color{0.5, 0.5, 0.5});
    world.add(std::make_shared<sphere>(point3{0.0, -1000.0, 0.0}, 1000.0, ground));

    point3 pt{4.0, 0.2, 0.0};
    for (i32 i = -11; i < 11; ++i) {
        for (i32 j = -11; j < 11; ++j) {
            auto rand_mat = randf32();
            point3 center{i + 0.9f * randf32(), 0.2f, j + 0.9f * randf32()};

            if ((center - pt).length() > 0.9) {
                std::shared_ptr<material> sphere_mat;

                if (rand_mat < 0.8) {
                    // diffuse
                    color c1 = randvec();
                    color c2 = randvec();
                    sphere_mat = std::make_shared<lambertian>(c1 * c2);
                } else if (rand_mat < 0.95) {
                    // metal
                    color albedo = randvec(0.5, 1);
                    f32 fuzz = randf32(0, 0.5);
                    sphere_mat = std::make_shared<metal>(albedo, fuzz);
                } else {
                    // glass
                    sphere_mat = std::make_shared<dielectric>(1.5);
                }
                world.add(std::make_shared<sphere>(center, 0.2, sphere_mat));
            }
        }
    }

    auto glass_mat = std::make_shared<dielectric>(1.5);
    auto flat_mat = std::make_shared<lambertian>(color{0.4, 0.2, 0.1});
    auto metal_mat = std::make_shared<metal>(color{0.7, 0.6, 0.5}, 0.0);

    world.add(std::make_shared<sphere>(point3{0.0, 1.0, 0.0}, 1.0, glass_mat));
    world.add(std::make_shared<sphere>(point3{-4.0, 1.0, 0.0}, 1.0, flat_mat));
    world.add(std::make_shared<sphere>(point3{4.0, 1.0, 0.0}, 1.0, metal_mat));

    return world;
}

/*****************************************************************************/

std::string process_rows(u32 tid, i32 start, i32 end, i32 img_height, i32 img_width, i32 samples, 
                         const camera& cam, const hittable_list& world, const i32 max_depth) {
    std::string pixels;
    
    for (i32 row = end - 1; row >= start; --row) {
        for (i32 col = 0; col < img_width; ++col) {
            color pixel;
            for (u32 s = 0; s < samples; ++s) {
                f32 u = (col + randf32()) / (img_width - 1);
                f32 v = (row + randf32()) / (img_height - 1);
                ray r = cam.get_ray(u, v);
                pixel += ray_color(r, world, max_depth);
            }
            pixels += pixel.get(samples);
        }
        pixels += '\n';
        ++g_counts[tid];
    }

    return pixels;
}
