include_directories(${RayTracer_SOURCE_DIR}/include)
include_directories(${RayTracer_SOURCE_DIR}/lib/stb)

file(GLOB RayTracer_SOURCE CONFIGURE_DEPENDS "*.cpp")

add_executable(RayTracer ${RayTracer_SOURCE})

find_package(Threads REQUIRED)

target_link_libraries(RayTracer PRIVATE 
    Threads::Threads 
    glm::glm)
