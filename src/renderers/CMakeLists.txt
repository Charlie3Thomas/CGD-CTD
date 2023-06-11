add_library(ct-renderers STATIC testrenderer.cpp)
target_include_directories(ct-renderers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-renderers PUBLIC ct-config ct-bvh ct-camera ct-embree ct-loaders ct-materials ct-utils tinyexr pthread)