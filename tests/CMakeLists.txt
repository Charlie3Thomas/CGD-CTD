add_test(NAME head     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-head-output.exr     -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj)
add_test(NAME head4k   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4khead-output.exr   -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj)
add_test(NAME teapot   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-output.exr   -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj)
add_test(NAME teapot4k COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kteapot-output.exr -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj)
add_test(NAME cow      COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-output.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj)
add_test(NAME cow4k    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kcow-output.exr    -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj)


add_executable(ct-test-textureloader testtextureload.cpp)
target_link_libraries(ct-test-textureloader PRIVATE freeimage ct-embree)

#add_executable(ct-test-boingo boingo.cpp)