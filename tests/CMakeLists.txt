add_test(NAME drag       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-double-dragon.exr            -d 1 -h 8 -i 1 -k -e 1) 
add_test(NAME stat       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-triple-statue.exr            -d 1 -h 8 -i 1 -k -e 2) 
add_test(NAME corn       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cornell-box.exr              -d 1 -h 8 -i 3 -k -e 3) 
add_test(NAME stat-al    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-tripple-statue-area-lit.exr  -d 1 -h 8 -i 1 -k -e 4) 
add_test(NAME drag-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-double-dragon-normals.exr    -d 1 -e 1 -n)
add_test(NAME stat-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-triple-statue-normals.exr    -d 1 -e 2 -n)
add_test(NAME corn-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cornell-box-normals.exr      -d 1 -e 3 -n)

