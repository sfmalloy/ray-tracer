cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

project(RayTracer)
add_subdirectory(lib/glm)
add_subdirectory(lib/yaml-cpp)
add_subdirectory(src)
