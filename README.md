# Simple Ray Tracer

Simple ray tracing engine adapted from [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Building
`cmake` is required for building. Currently this engine does not rely on outside libraries (except pthreads, but more will be added). To build

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
