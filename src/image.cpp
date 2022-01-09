#include <vector>

#include "types.h"
#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

bool write_to_png(const char* filename, const std::vector<u8color>& pixels, i32 img_width, i32 img_height) {
    i32 d = stbi_write_png(filename, img_width, img_height, COMP, pixels.data(), img_width * COMP);
    return d;
}

bool write_to_jpg(const char* filename, const std::vector<u8color>& pixels, i32 img_width, i32 img_height) {
    i32 d = stbi_write_jpg(filename, img_width, img_height, COMP, pixels.data(), 100);
    return d;
}
