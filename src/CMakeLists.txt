add_library(helper-functions STATIC helper_functions.cpp)
target_include_directories(helper-functions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})