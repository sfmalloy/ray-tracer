#pragma once

#include "types.hpp"
#include "camera.hpp"
#include "hittable_list.hpp"

struct scene_attributes {
    i32 aspect_width;
    i32 aspect_height;
    i32 img_height;
    i32 img_width;
    u32 samples;
    i32 max_depth;
    camera cam;
    hittable_list world;
    color background;
};
