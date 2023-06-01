add_executable(ray-tracer main.cpp)
find_package(CGAL REQUIRED)
target_link_libraries(ray-tracer PRIVATE CGAL::CGAL ct-utils)
