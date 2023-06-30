add_library(ct-light STATIC light.hpp light.cpp)
find_package (Eigen3 3.3 REQUIRED)
target_include_directories(ct-light PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-light PUBLIC Eigen3::Eigen)