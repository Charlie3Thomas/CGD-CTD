add_library(ct-embree STATIC embreesingleton.cpp)
find_package(embree 3.0 REQUIRED)
target_include_directories(ct-embree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-embree PUBLIC embree ct-materials)