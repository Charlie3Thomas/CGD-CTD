add_executable(ray-tracer main.cpp)
find_package(embree 3.0 REQUIRED)
target_link_libraries(ray-tracer PRIVATE embree ct-utils)
