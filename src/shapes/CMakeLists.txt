add_library(ct-shapes STATIC cylinder.cpp disk.cpp parallelepiped.cpp parellelograpm.cpp polygon.cpp sphere.cpp triangle.cpp tube.cpp) 
target_include_directories(ct-shapes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-shapes PUBLIC ct-common)