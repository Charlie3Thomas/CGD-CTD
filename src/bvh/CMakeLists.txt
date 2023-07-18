add_library(ct-bvh bvh.cpp)
target_include_directories(ct-bvh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-bvh PUBLIC ct-embree ct-utils)