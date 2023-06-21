add_library(ct-texture STATIC texture.cpp)
find_package (Eigen3 3.3 REQUIRED)
target_include_directories(ct-texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-texture PUBLIC freeimage ct-utils)