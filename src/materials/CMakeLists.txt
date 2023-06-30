add_library(ct-materials STATIC material.hpp brdfdata.hpp)
find_package (Eigen3 3.3 REQUIRED)
target_include_directories(ct-materials PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-materials PUBLIC ct-utils Eigen3::Eigen)