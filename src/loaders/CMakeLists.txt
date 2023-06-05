find_package(assimp CONFIG REQUIRED)
add_library(ct-loaders STATIC objloader.cpp textureloader.cpp)
target_include_directories(ct-loaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-loaders PRIVATE assimp freeimage ct-embree)