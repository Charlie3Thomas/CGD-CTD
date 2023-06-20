#TODO : More sensible test structure

## COW
# 1080p cow no bvh
add_test(NAME cube-2160              COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cube-2160.exr        -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cube.obj -t 20  -b       )
add_test(NAME cow-1080               COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080.exr         -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20 -b         )
add_test(NAME woody-2160             COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-woody-1080.exr       -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/woody.obj -t 20 -b       )
add_test(NAME cow-1080-b0c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080.exr         -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20            )
# 1080p cow with bvh-1080  
add_test(NAME cow-1080-b1c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080-b.exr       -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20 -b         )
# 1080p cow with visualisat  
add_test(NAME cow-1080-b1c1n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080-bc.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20 -b -c      )
add_test(NAME cow-1080-b1c0n1        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080-bn.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20 -b -n      )     
# 2160p cow no bv  
add_test(NAME cow-2160-b0c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160.exr         -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20            )
# 2160p cow with bvh   
add_test(NAME cow-2160-b1c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-b.exr       -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -t 20            ) 
## TEAPOT   
# 1080p teapot no   
add_test(NAME teapot-1080-b0c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -t 20         )
# 1080p teapot with bvh-   
add_test(NAME teapot-1080-b1c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080-b.exr    -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -t 20 -b      )
# 1080p teapot with visualisat   
add_test(NAME teapot-1080-b1c1n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080-bc.exr   -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -t 20 -b -c   )
add_test(NAME teapot-1080-b1c0n1     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080-bn.exr   -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -t 20 -b -n   )
# 2160p teapot no   
add_test(NAME teapot-2160-b0c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-2160.exr      -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -t 20         )
# 2160p teapot with   
add_test(NAME teapot-2160-b1c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-2160-b.exr    -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -t 20 -b      )

add_test(NAME triangle-2160-b1c0n0   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-triangle-2160-b.exr  -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/triangle.obj -t 20 -b    )