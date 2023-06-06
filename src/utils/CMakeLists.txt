add_library(ct-utils ppm.cpp exr.cpp)
target_include_directories(ct-utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(ray-tracer INTERFACE ct-utils)