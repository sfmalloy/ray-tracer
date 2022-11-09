#include <iterator>
#include <numeric>
#include <thread>
#include <iostream>
#include <vector>

#include "color.hpp"
#include "scene_attributes.hpp"
#include "renderer.hpp"
#include "types.hpp"
#include "utils.hpp"

color renderer::ray_color(const ray& r, const hittable& world, u32 depth) {
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

row_renderer::row_renderer(u32 num_threads)
  : m_num_threads(num_threads)
  , m_line_counts()
{ }

std::vector<u8color> row_renderer::render(const scene_attributes& scene) {
    std::vector<u8color> pixels;
    if (m_num_threads == 1) {
        render_thread(0, 0, scene.img_height, scene);
    } else {
        std::vector<std::future<std::vector<u8color>>> futures;
        futures.reserve(m_num_threads);
        m_line_counts = std::vector<u32>(m_num_threads, 0);
        for (u32 tid = 0; tid < m_num_threads; ++tid) {
            u32 start = partition(tid, m_num_threads, scene.img_height);
            u32 end = partition(tid + 1, m_num_threads, scene.img_height);
            futures.push_back(std::async(&row_renderer::render_thread, this, tid, start, end, std::cref(scene)));
        }

        std::cerr << "Num threads: " << futures.size() << '\n';
        i32 sum;
        do {
            sum = std::accumulate(std::begin(m_line_counts), std::end(m_line_counts), 0);
            std::cerr << "\rScanlines finished: " << sum << std::flush;
        } while (sum < scene.img_height);

        for (auto it = std::rbegin(futures); it != std::rend(futures); ++it) {
            auto row_data = it->get();
            pixels.insert(std::end(pixels), std::begin(row_data), std::end(row_data));
        }
    }
    return pixels;
}

std::vector<u8color> row_renderer::render_thread(u32 tid, i32 start, i32 end, const scene_attributes& scene) {
    std::vector<u8color> pixels;
    for (i32 row = end - 1; row >= start; --row) {
        for (i32 col = 0; col < scene.img_width; ++col) {
            color pixel{};
            for (u32 s = 0; s < scene.samples; ++s) {
                f32 u = (col + randf32()) / (scene.img_width - 1);
                f32 v = (row + randf32()) / (scene.img_height - 1);
                ray r = scene.cam.get_ray(u, v);
                pixel += ray_color(r, scene.world, scene.max_depth);
            }
            pixels.push_back(to_u8color(pixel, scene.samples));
        }
        ++m_line_counts[tid];
    }
    return pixels;
}

std::vector<u8color> tile_renderer::render(const scene_attributes& scene) {
    std::vector<u8color> pixels;
    return pixels;
}
