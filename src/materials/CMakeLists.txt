add_library(ct-materials STATIC material.cpp)
target_include_directories(ct-materials PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ct-materials PUBLIC ct-utils)