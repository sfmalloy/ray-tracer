#include <numeric>
#include <thread>
#include <future>
#include <numeric>

#include <fmt/core.h>

#include "color.hpp"
#include "renderer.hpp"
#include "utils.hpp"

color renderer::ray_color(const ray& r, const color& background, const hittable& world, u32 depth) {
    if (depth <= 0)
        return color{};
    
    hit_record rec;
    if (!world.hit(r, 0.001f, INF, rec)) {
        return background;
    }

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

row_renderer::row_renderer(u32 num_threads)
  : m_num_threads{num_threads}
  , m_counts{}
{ }

std::vector<u8color> row_renderer::render(const scene_attributes& scene) {
    fmt::print("\e[?25l");

    std::vector<u8color> pixels;
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
                pixel += ray_color(r, scene.background, scene.world, scene.max_depth);
            }
            pixels.push_back(to_u8color(pixel, scene.samples));
        }
        ++m_counts[tid];
    }
    return pixels;
}

tile_renderer::tile_renderer(u32 num_threads)
  : m_num_threads{num_threads}
  , m_grid{}
  , m_lock{}
  , m_tiles{}
  , m_counts{}
{ }

std::vector<u8color> tile_renderer::render(const scene_attributes& scene) {
    fmt::print("\e[?25l");
    u32 tile_limit = m_num_threads;
    for (i32 r = 0; r < tile_limit; ++r) {
        u32 r_start = partition(r, tile_limit, scene.img_height);
        u32 r_end = partition(r + 1, tile_limit, scene.img_height);
        for (i32 c = 0; c < tile_limit; ++c) {
            u32 c_start = partition(c, tile_limit, scene.img_width);
            u32 c_end = partition(c + 1, tile_limit, scene.img_width);
            m_tiles.emplace(r_start, r_end, c_start, c_end);
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
                    pixel += ray_color(r, scene.background, scene.world, scene.max_depth);
                }
                m_grid[row][col] = to_u8color(pixel, scene.samples);
            }
        }
        ++m_counts[tid];
    }
}
