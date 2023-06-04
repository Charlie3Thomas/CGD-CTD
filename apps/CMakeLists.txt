add_executable(ray-tracer main.cpp)
find_package(embree 3.0 REQUIRED)
find_package(assimp REQUIRED)
target_link_libraries(ray-tracer PRIVATE embree assimp ct-utils)
