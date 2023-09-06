add_test(NAME drag          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-double-dragon.exr            -p 1 -d 1 -h 1 -i 1 -k -e 1 -m) 
add_test(NAME stat          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-triple-statue.exr            -p 1 -d 1 -h 1 -i 1 -k -e 2 -m) 
add_test(NAME corn          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-cornell-box.exr              -p 1 -d 1 -h 1 -i 1 -k -e 3 -m) 
add_test(NAME stat-al       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-tripple-statue-area-lit.exr  -p 4 -d 32 -h 16 -i 3 -k -e 4 -m) 
add_test(NAME split         COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-split-room.exr               -p 1 -d 1 -h 1 -i 1 -k -e 5 -m)
add_test(NAME split-l       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-split-room-l.exr             -p 1 -d 1 -h 1 -i 1 -k -e 6 -m)
add_test(NAME split-r       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/min-k-split-room-r.exr             -p 1 -d 1 -h 1 -i 1 -k -e 7 -m)

add_test(NAME teapot        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot.exr                    -p 16 -d 16 -h 16 -i 3 -e 8 -k -m)


add_test(NAME drag-norms    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-double-dragon-normals.exr    -p 1 -d 4 -e 1 -m -n)
add_test(NAME stat-norms    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-triple-statue-normals.exr    -p 1 -d 4 -e 2 -m -n)
add_test(NAME corn-norms    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cornell-box-normals.exr      -p 1 -d 4 -e 3 -m -n)
add_test(NAME split-norms   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-split-room-normals.exr       -p 1 -d 4 -h 1 -i 1 -k -e 5 -m -n)
add_test(NAME split-l-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-split-room-l-normals.exr     -p 1 -d 4 -h 1 -i 1 -k -e 6 -m -n)
add_test(NAME split-r-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-split-room-r-normals.exr     -p 1 -d 4 -h 1 -i 1 -k -e 7 -m -n)


add_test(NAME ref-drag          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-double-dragon.exr            -p 32 -d 16 -h 16 -i 3 -e 1 -m) 
add_test(NAME ref-stat          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-triple-statue.exr            -p 64 -d 16 -h 16 -i 3 -e 2 -m) 
add_test(NAME ref-corn          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-cornell-box.exr              -p 64 -d 16 -h 16 -i 3 -e 3 -m) 
add_test(NAME ref-stat-al       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-tripple-statue-area-lit.exr  -p 32 -d 32 -h 32 -i 3 -e 4 -m) 
add_test(NAME ref-split         COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-split-room.exr               -p 64 -d 16 -h 32 -i 3 -e 5 -m)
add_test(NAME ref-split-l       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-split-room-l.exr             -p 16 -d 16 -h 16 -i 3 -e 6 -m)
add_test(NAME ref-split-r       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/ref-split-room-r.exr             -p 64 -d 16 -h 32 -i 3 -e 7 -m)