#pragma once

#include <vector>
#include "color.h"
#include "types.h"

constexpr const i32 COMP = 3;

bool write_to_png(const char* filename, const std::vector<u8color>& pixels, i32 img_width, i32 img_height);
