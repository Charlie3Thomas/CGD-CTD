add_library(ct-scene STATIC test_scenes.cpp)
target_include_directories(ct-scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-scene PUBLIC ct-materials ct-lights ct-shapes)