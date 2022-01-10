#pragma once

#include <iostream>

#include <memory>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include "types.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "hittable_list.hpp"

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
            if (node["type"].as<std::string>() == "sphere")
                rhs = node.as<std::shared_ptr<sphere>>();
            else
                return false;
            return true;
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
            } else if (type == "metal") {
                rhs = std::make_shared<metal>(node["color"].as<color>(), node["fuzz"].as<f32>());
            } else {
                return false;
            }
            return true;
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
}
