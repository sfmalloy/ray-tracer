#include <iterator>
#include <numeric>
#include <thread>
#include <iostream>
#include <vector>
#include <numeric>
#include <future>

#include <fmt/core.h>

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
  , m_counts()
{ }

std::vector<u8color> row_renderer::render(const scene_attributes& scene) {
    fmt::print("\e[?25l");
    std::vector<u8color> pixels;
    // if (m_num_threads == 1) {
        // render_thread(0, 0, scene.img_height, scene);
    // } else {
        std::vector<std::future<std::vector<u8color>>> futures;
        futures.reserve(m_num_threads);
        m_counts = std::vector<u32>(m_num_threads, 0);
        for (u32 tid = 0; tid < m_num_threads; ++tid) {
            u32 start = partition(tid, m_num_threads, scene.img_height);
            u32 end = partition(tid + 1, m_num_threads, scene.img_height);
            futures.push_back(std::async(&row_renderer::render_thread, this, tid, start, end, std::cref(scene)));
        }
        i32 sum;
        do {
            sum = std::accumulate(std::begin(m_counts), std::end(m_counts), 0);
            fmt::print("\rLines rendered: {}/{}", sum, scene.img_height);
        } while (sum < scene.img_height);

        for (auto it = std::rbegin(futures); it != std::rend(futures); ++it) {
            auto row_data = it->get();
            pixels.insert(std::end(pixels), std::begin(row_data), std::end(row_data));
        }
    // }
    fmt::print("\e[?25h");

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
        ++m_counts[tid];
    }
    return pixels;
}

tile_renderer::tile_renderer(u32 num_threads)
  : m_num_threads(num_threads)
  , m_grid()
  , m_lock()
  , m_tiles()
  , m_counts()
{ }

std::vector<u8color> tile_renderer::render(const scene_attributes& scene) {
    fmt::print("\e[?25l");
    // TODO: use partition here instead
    u32 w = scene.img_width;
    u32 f = m_num_threads;
    while (w % f != 0) {
        ++f;
    }
    w /= f;

    u32 h = scene.img_height;
    f = m_num_threads;
    while (h % f != 0) {
        ++f;
    }
    h /= f;
    u32 pixels_needed = scene.img_height * scene.img_width;
    u32 pc = 0;
    for (i32 r = scene.img_height - h; r >= 0; r -= h) {
        for (i32 c = 0; c < scene.img_width; c += w) {
            m_tiles.emplace(r, r + h, c, c + w);
            pc += (r + h - r) * (c + w - c);
        }
    }
    u32 total_tiles = m_tiles.size();
    m_counts = std::vector<u32>(m_num_threads, 0);
    m_grid = std::vector<std::vector<u8color>>(scene.img_height, std::vector<u8color>(scene.img_width));

    std::vector<std::thread> threads;
    threads.reserve(m_num_threads);
    m_counts = std::vector<u32>(m_num_threads, 0);
    for (u32 tid = 0; tid < m_num_threads; ++tid) {
        threads.emplace_back(&tile_renderer::render_thread, this, tid, scene);
    }

    u32 tile_count;
    do {
        tile_count = std::accumulate(std::begin(m_counts), std::end(m_counts), 0);
        fmt::print("\rTiles rendered: {}/{}", tile_count, total_tiles);
    } while (tile_count < total_tiles);

    for (auto& t : threads) {
        t.join();
    }

    std::vector<u8color> pixels;
    for (auto it = std::rbegin(m_grid); it != std::rend(m_grid); ++it) {
        pixels.insert(std::end(pixels), std::begin(*it), std::end(*it));
    }
    fmt::print("\e[?25h");
    return pixels;
}

void tile_renderer::render_thread(u32 tid, const scene_attributes& scene) {
    while (m_tiles.size() > 0) {
        m_lock.lock();
        auto top = m_tiles.top();
        m_tiles.pop();
        m_lock.unlock();

        i32 r_start = top[0];
        i32 r_end = top[1];
        i32 c_start = top[2];
        i32 c_end = top[3];

        for (i32 row = r_start; row < r_end; ++row) {
            for (i32 col = c_start; col < c_end; ++col) {
                color pixel{};
                for (u32 s = 0; s < scene.samples; ++s) {
                    f32 u = (col + randf32()) / (scene.img_width - 1);
                    f32 v = (row + randf32()) / (scene.img_height - 1);
                    ray r = scene.cam.get_ray(u, v);
                    pixel += ray_color(r, scene.world, scene.max_depth);
                }
                m_grid[row][col] = to_u8color(pixel, scene.samples);
            }
        }
        ++m_counts[tid];
    }
}
