add_library(ct-light STATIC light.hpp)
find_package (Eigen3 3.3 REQUIRED)
target_include_directories(ct-light PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-light PUBLIC ct-loaders ct-utils Eigen3::Eigen)