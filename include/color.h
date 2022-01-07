#pragma once

#include "types.h"
#include "vec3.h"

class color : public vec3 {
public:
    color();
    color(const vec3& v);
    color(f32 r, f32 g, f32 b);
    void write(std::ostream& out, u32 samples);
    void write_gray(std::ostream&out, u32 samples);
    std::string get(u32 samples) const;
    
    f32 r() const;
    f32 g() const;
    f32 b() const;
};
