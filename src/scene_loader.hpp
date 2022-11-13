#pragma once

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include "glm/fwd.hpp"
#include "scene_attributes.hpp"
#include "types.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "hittable_list.hpp"

hittable_list random_scene();

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;
            rhs.x = node[0].as<f32>();
            rhs.y = node[1].as<f32>();
            rhs.z = node[2].as<f32>();
            return true;
        }
    };

    template<>
    struct convert<std::shared_ptr<hittable>> {
        static bool decode(const Node& node, std::shared_ptr<hittable>& rhs) {
            if (node["type"].as<std::string>() == "sphere") {
                rhs = node.as<std::shared_ptr<sphere>>();
                return true;
            }
            return false;
        }
    };

    template<>
    struct convert<hittable_list> {
        static bool decode(const Node& node, hittable_list& rhs) {
            if (!node.IsSequence())
                return false;
            rhs = hittable_list{};
            for (auto& obj : node) {
                rhs.add(obj.as<std::shared_ptr<hittable>>());
            }
            return true;
        }
    };
    
    template<>
    struct convert<std::shared_ptr<material>> {
        static bool decode(const Node& node, std::shared_ptr<material>& rhs) {
            std::string type = node["type"].as<std::string>();
            if (type == "lambertian") {
                rhs = std::make_shared<lambertian>(node["color"].as<color>());
                return true;
            } else if (type == "metal") {
                rhs = std::make_shared<metal>(node["color"].as<color>(), node["fuzz"].as<f32>());
                return true;
            } else if (type == "dielectric") {
                rhs = std::make_shared<dielectric>(node["index"].as<f32>());
                return true;
            }
            return false;
        }
    };

    template<>
    struct convert<std::shared_ptr<sphere>> {
        static bool decode(const Node& node, std::shared_ptr<sphere>& rhs) {
            rhs = std::make_shared<sphere>(
                node["center"].as<glm::vec3>(),
                node["radius"].as<f32>(),
                node["material"].as<std::shared_ptr<material>>()
            );
            return true;
        }
    };

    template<>
    struct convert<scene_attributes> {
        static bool decode(const Node& node, scene_attributes& rhs) {
            if (node["scene"].IsDefined()) {
                rhs.world = node["scene"].as<hittable_list>();
            } else {
                rhs.world = random_scene();
            }

            if (node["settings"].IsDefined()) {
                if (!node["settings"].IsMap())
                    return false;
                auto settings = node["settings"];
                if (settings["aspect_ratio"].IsDefined()) {
                    if (!settings["aspect_ratio"].IsSequence())
                        return false;
                    rhs.aspect_width = settings["aspect_ratio"][0].as<i32>();
                    rhs.aspect_height = settings["aspect_ratio"][1].as<i32>();
                } else {
                    rhs.aspect_width = 1;
                    rhs.aspect_height = 1;
                }

                if (settings["image_width"].IsDefined()) {
                    rhs.img_width = settings["image_width"].as<i32>();
                } else {
                    rhs.img_width = 500;
                }
                f32 aspect_ratio = static_cast<f32>(rhs.aspect_width) / rhs.aspect_height;
                rhs.img_height = static_cast<i32>(rhs.img_width / aspect_ratio);

                if (settings["samples"].IsDefined()) {
                    if (!settings["samples"].IsScalar())
                        return false;
                    rhs.samples = settings["samples"].as<u32>();
                } else {
                    rhs.samples = 5;
                }

                if (settings["depth"].IsDefined()) {
                    if (!settings["depth"].IsScalar())
                        return false;
                    rhs.max_depth = settings["depth"].as<i32>();
                } else {
                    rhs.max_depth = 10;
                }

                glm::vec3 lookfrom;
                if (settings["lookfrom"].IsDefined()) {
                    lookfrom = settings["lookfrom"].as<glm::vec3>();
                } else {
                    lookfrom = { 13.0f, 2.0f, 3.0f };
                }

                glm::vec3 lookat;
                if (settings["lookat"].IsDefined()) {
                    lookat = settings["lookat"].as<glm::vec3>();
                } else {
                    lookat = { 0.0f, 0.0f, 0.0f };
                }
                
                glm::vec3 up;
                if (settings["up"].IsDefined()) {
                    up = settings["up"].as<glm::vec3>();
                } else {
                    up = { 0.0f, 1.0f, 0.0f };
                }

                f32 fov;
                if (settings["fov"].IsDefined()) {
                    fov = settings["fov"].as<f32>();
                } else {
                    fov = 20.0f;
                }

                f32 focus_distance;
                if (settings["focus_distance"].IsDefined()) {
                    focus_distance = settings["focus_distance"].as<f32>();
                } else {
                    focus_distance = 10.0f;
                }

                f32 aperture;
                if (settings["aperture"].IsDefined()) {
                    aperture = settings["aperture"].as<f32>();
                } else {
                    aperture = 0.1f;
                }

                rhs.cam = camera{
                    lookfrom,
                    lookat,
                    up,
                    fov,
                    aspect_ratio,
                    aperture,
                    focus_distance
                };
            } else {
                std::cout << "LOADING DEFAULT SCENE\n";
                rhs = scene_attributes{
                    .aspect_width = 1, 
                    .aspect_height = 1, 
                    .img_height = 500, 
                    .img_width = 500, 
                    .samples = 50, 
                    .max_depth = 10,
                    .cam = camera{
                        { 13.0f, 2.0f, 3.0f },
                        { 0.0f, 0.0f, 0.0f },
                        { 0.0f, 1.0f, 0.0f },
                        20.0f, 1.0f, 0.1f, 10.0f
                    },
                    .world = random_scene()
                };
            }

            return true;
        }
    };
}
