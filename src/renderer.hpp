#pragma once

#include <future>
#include <thread>

#include "hittable.hpp"
#include "scene_attributes.hpp"
#include "timer.hpp"
#include "types.hpp"

class renderer {
public:
    virtual std::vector<u8color> render(const scene_attributes& attrs) = 0;
    color ray_color(const ray& r, const hittable& world, u32 depth);
};

class row_renderer : public renderer {
public:
    row_renderer() = delete;
    row_renderer(u32 num_threads);
    row_renderer(const row_renderer&) = delete;
    row_renderer operator=(const row_renderer&) = delete;

    std::vector<u8color> render(const scene_attributes& scene);
private:
    std::vector<u8color> render_thread(u32 tid, i32 start, i32 end, const scene_attributes& scene);
    u32 m_num_threads;
    std::vector<u32> m_line_counts;
};

class tile_renderer : public renderer {
public:
    tile_renderer() = delete;
    tile_renderer(u32 num_threads);
    tile_renderer(tile_renderer&) = delete;
    tile_renderer operator=(const tile_renderer&) = delete;

    std::vector<u8color> render(const scene_attributes& scene);
private:
    u32 m_num_threads;
};
