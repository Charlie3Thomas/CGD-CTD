add_library(ct-image STATIC image.cpp)
target_include_directories(ct-image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})