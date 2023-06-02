add_library(ct-utils ppm.cpp)
target_include_directories(ct-utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)