add_executable(ray-tracer CTRT.cpp)
target_link_libraries(ray-tracer PRIVATE ct-config ct-bvh ct-embree ct-loaders ct-utils tinyexr)
