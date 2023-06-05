#add_test(NAME sphere        COMMAND ray-tracer -f png -o ${CMAKE_SOURCE_DIR}/renders/test-sphere.png        -r 1920x1080 -s sphere)
#add_test(NAME 4k-sphere     COMMAND ray-tracer -f png -o ${CMAKE_SOURCE_DIR}/renders/test-4k-sphere.png     -r 3840x2160 -s sphere)
#add_test(NAME cylinders     COMMAND ray-tracer -f png -o ${CMAKE_SOURCE_DIR}/renders/test-cylinders.png     -r 1920x1080 -s cylinders)
#add_test(NAME 4k-cylinders  COMMAND ray-tracer -f png -o ${CMAKE_SOURCE_DIR}/renders/test-4k-cylinders.png  -r 3840x2160 -s cylinders)

add_executable(ct-test-textureloader testtextureload.cpp)
target_link_libraries(ct-test-textureloader PRIVATE freeimage ct-embree)

#add_executable(ct-test-boingo boingo.cpp)