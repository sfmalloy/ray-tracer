/*****************************************************************************/
// System includes
#include <iostream>
#include <string>
#include <csignal>

/*****************************************************************************/
// Lib includes
#include <fmt/core.h>
#include <cxxopts.hpp>
#include <yaml-cpp/yaml.h>

/*****************************************************************************/
// Local includes
#include "image.hpp"
#include "renderer.hpp"
#include "scene_attributes.hpp"
#include "timer.hpp"
#include "types.hpp"
#include "yaml-cpp/node/parse.h"
#include "scene_loader.hpp"

/*****************************************************************************/

void signal_handler(i32 signal) {
    fmt::print("\r\e[?25h\n");
    exit(-1);
}

int main(int argc, char** argv) {
    std::signal(SIGINT, signal_handler);

    cxxopts::Options opts{"RayTracer", "Simple RayTracing engine based on \"Ray Tracing in One Weekend\""};
    opts.add_options()
        ("o,outfile", "Output file (either .jpg or .png)", cxxopts::value<std::string>()->default_value("out.png"), "<path>")
        ("t,threads", "Number of threads for rendering", cxxopts::value<u32>()->default_value("1"), "<count>")
        ("r,renderer", "Either \"tile\" or \"row\"", cxxopts::value<std::string>()->default_value("row"), "<type>")
        ("s,scene", "Scene file (.yml format)", cxxopts::value<std::string>()->default_value(""), "<name>")
        ("h,help", "Print usage");
    opts.allow_unrecognised_options();

    auto args = opts.parse(argc, argv);
    if (args.count("help")) {
        fmt::print("{}\n", opts.help());
        return 0;
    }

    YAML::Node node;
    auto scene_name = args["scene"].as<std::string>();
    if (scene_name.length() > 0) {
        node = YAML::LoadFile(scene_name);
    } else {
        node = YAML::Load(scene_name);
    }
    auto scene = node.as<scene_attributes>();

    renderer* r;
    auto render_type = args["renderer"].as<std::string>();
    auto num_threads = args["threads"].as<u32>();
    if (render_type == "row")
        r = new row_renderer(num_threads);
    else
        r = new tile_renderer(num_threads);
    timer t;
    t.start();
    auto pixel_data = r->render(scene);

    auto outfile = args["outfile"].as<std::string>();
    fmt::print("\nWriting to {}\n", outfile);
    if (outfile.substr(outfile.find(".")) == ".png") {
        if (!write_to_png(outfile, pixel_data, scene.img_width, scene.img_height)) {
            fmt::print("Failed to write {}\n", outfile);
            return -1;
        }
    } else {
        if (!write_to_jpg(outfile, pixel_data, scene.img_width, scene.img_height)) {
            fmt::print("Failed to write {}\n", outfile);
            return -1;
        }
    }
    t.stop();
    fmt::print("Time: {:.3f}s\n", t.elapsed_seconds());

    delete r;
    return 0;
}
