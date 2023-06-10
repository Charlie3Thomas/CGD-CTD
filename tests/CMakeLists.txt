add_test(NAME head     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-head-output.exr     -r 1000x1000 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj)
add_test(NAME head4k   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4khead-output.exr   -r 4000x4000 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj)
add_test(NAME teapot   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-output.exr   -r 1000x1000 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj)
add_test(NAME teapot4k COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kteapot-output.exr -r 4000x4000 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj)


add_executable(ct-test-textureloader testtextureload.cpp)
target_link_libraries(ct-test-textureloader PRIVATE freeimage ct-embree)

#add_executable(ct-test-boingo boingo.cpp)