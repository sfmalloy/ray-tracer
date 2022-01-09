#pragma once

#include <ostream>
#include <glm/vec3.hpp>

#include "types.h"

void write_color(std::ostream& out, const color& c, u32 samples);

void write_color_gray(std::ostream& out, const color& c, u32 samples);

std::string color_string(const color& c, u32 samples);
