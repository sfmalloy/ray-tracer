# Simple Ray Tracer

Simple ray tracing engine adapted from [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Cloning
To make sure you're using the same versions of submodules I am, you should clone using the `--recursive` flag
```
git clone https://github.com/sfmalloy/ray-tracer.git --recursive
```

## Building
Current outside libraries include:
- glm
- Whatever threading library your compiler uses to implement std::future

`cmake` is required for building. To build
```
mkdir build
cd build
cmake ..
make
```
## Running
```
./src/RayTracer <image_name>.ppm <thread_count>
```
To change settings, currently you have to edit render and camera settings within `main.cpp`, but eventually I will add a way to do this from a seperate config file.

![Rendered image of a bunch of spheres](img/final.png)
