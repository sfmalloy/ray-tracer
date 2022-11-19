#pragma once

#include <vector>

#include "color.hpp"
#include "types.hpp"

constexpr const i32 COMP = 3;

bool write_to_png(const std::string& filename, const std::vector<u8color>& pixels, i32 img_width, i32 img_height);

bool write_to_jpg(const std::string& filename, const std::vector<u8color>& pixels, i32 img_width, i32 img_height);
