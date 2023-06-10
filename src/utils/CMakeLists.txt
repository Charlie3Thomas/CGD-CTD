add_library(ct-utils ppm.cpp exr.cpp timer.cpp)
find_package(embree 3.0 REQUIRED)
target_include_directories(ct-utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-utils PUBLIC embree)