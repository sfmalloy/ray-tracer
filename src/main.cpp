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
        std::cerr << "Please enter filename\nUsage: ./RayTracer <filename>.png <thread_count> [<scene>.yml]\n";
        return -1;
    }
    std::srand(std::time(0));

  
    timer t;
    YAML::Node node;
    if (argc >= 4) {
        node = YAML::LoadFile(argv[3]);
    }
    scene_attributes scene = node.as<scene_attributes>();

    // hide terminal cursor
    std::printf("\e[?25l");

    t.start();
    u32 thread_count = std::stoi(std::string{argv[2]});
    // tile_renderer renderer(thread_count);
    row_renderer renderer(thread_count);
    auto pixel_data = renderer.render(scene);
    if (pixel_data.size() > 0) {
        if (!write_to_png(argv[1], pixel_data, scene.img_width, scene.img_height))
            std::cerr << "\nWrite fail";
    }
    t.stop();

    // re-show terminal cursor
    std::printf("\e[?25h");
    std::cerr << "\nTime: " << std::fixed << std::setprecision(3) << t.elapsed_seconds() << "s\n";

    return 0;
}
