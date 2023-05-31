add_executable(hello-world main.cpp)
target_link_libraries(hello-world PRIVATE pthread freeimage ct-ppm)