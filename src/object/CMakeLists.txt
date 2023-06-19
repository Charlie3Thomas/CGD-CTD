add_library(ct-object STATIC object.cpp oingoscene.cpp)
target_include_directories(ct-object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-object PUBLIC ct-materials ct-textures embree)
