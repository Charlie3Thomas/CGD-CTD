add_library(ct-math INTERFACE)
target_include_directories(ct-math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_features(ct-math INTERFACE cxx_std_20)
