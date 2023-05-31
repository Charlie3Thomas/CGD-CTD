add_library(ct-cameras STATIC digital.cpp pinhole.cpp)
target_include_directories(ct-cameras PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-cameras PUBLIC ct-common)