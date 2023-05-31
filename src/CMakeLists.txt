add_library(ct-ppm STATIC ppm.cpp)
target_include_directories(ct-ppm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-ppm PUBLIC ct-math)
add_subdirectory(math)