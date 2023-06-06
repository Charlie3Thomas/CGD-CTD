add_library(ct-config STATIC options.cpp)
target_include_directories(ct-config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(ct-config PUBLIC cxx_std_20)