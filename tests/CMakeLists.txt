#TODO : More sensible test structure

## COW
# 1080p cow no bvh
add_test(NAME cow-1080-b0c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080.exr         -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj            )
# 1080p cow with bvh-1080  
add_test(NAME cow-1080-b1c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080-b.exr       -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b         )
# 1080p cow with visualisat  
add_test(NAME cow-1080-b1c1n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080-bc.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -c      )
add_test(NAME cow-1080-b1c0n1        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-1080-bn.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -n      )     
# 2160p cow no bv  
add_test(NAME cow-2160-b0c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160.exr         -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj            )
# 2160p cow with bvh   
add_test(NAME cow-2160-b1c0n0        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-b.exr       -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj            ) 
## TEAPOT   
# 1080p teapot no   
add_test(NAME teapot-1080-b0c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080.exr      -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj         )
# 1080p teapot with bvh-   
add_test(NAME teapot-1080-b1c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080-b.exr    -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b      )
# 1080p teapot with visualisat   
add_test(NAME teapot-1080-b1c1n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080-bc.exr   -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -c   )
add_test(NAME teapot-1080-b1c0n1     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-1080-bn.exr   -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -n   )
# 2160p teapot no   
add_test(NAME teapot-2160-b0c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-2160.exr      -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj         )
# 2160p teapot with   
add_test(NAME teapot-2160-b1c0n0     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-2160-b.exr    -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b      )

# 4320p cow no bvh thread tests
add_test(NAME cow-4320-t01-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 1 )
add_test(NAME cow-4320-t02-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 2 )
add_test(NAME cow-4320-t04-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 4 )
add_test(NAME cow-4320-t08-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 8 )
add_test(NAME cow-4320-t12-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 12)
add_test(NAME cow-4320-t16-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 16)
add_test(NAME cow-4320-t20-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320.exr   -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj    -t 20)
# 4320p cow   -s 100x100
add_test(NAME cow-4320-t01-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 1 )
add_test(NAME cow-4320-t02-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 )
add_test(NAME cow-4320-t04-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 )
add_test(NAME cow-4320-t08-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 )
add_test(NAME cow-4320-t12-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12)
add_test(NAME cow-4320-t16-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16)
add_test(NAME cow-4320-t20-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-4320-b.exr -r 7680x4320 -s 40x40 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20)


# 2160p cow with bvh 2 thread canvas size tests
add_test(NAME cow-2160-t2-ca4x4     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca0.exr  -r 3840x2160 -s 4x4     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca8x8     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca1.exr  -r 3840x2160 -s 8x8     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca10x10   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca2.exr  -r 3840x2160 -s 10x10   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca12x12   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca3.exr  -r 3840x2160 -s 12x12   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca16x16   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca4.exr  -r 3840x2160 -s 16x16   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca20x20   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca5.exr  -r 3840x2160 -s 20x20   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca24x24   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca6.exr  -r 3840x2160 -s 24x24   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca30x30   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca7.exr  -r 3840x2160 -s 30x30   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca40x40   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca8.exr  -r 3840x2160 -s 40x40   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca48x48   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca9.exr  -r 3840x2160 -s 48x48   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca60x60   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca10.exr -r 3840x2160 -s 60x60   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca80x80   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca11.exr -r 3840x2160 -s 80x80   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca120x120 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca12.exr -r 3840x2160 -s 120x120 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca240x240 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca13.exr -r 3840x2160 -s 240x240 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca480x360 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca14.exr -r 3840x2160 -s 480x360 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca768x540 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca15.exr -r 3840x2160 -s 768x540 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)
add_test(NAME cow-2160-t2-ca960x720 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca16.exr -r 3840x2160 -s 960x720 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 2 -c)

# 2160p cow with bvh 4 thread canvas size tests
add_test(NAME cow-2160-t4-ca4x4     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca0.exr  -r 3840x2160 -s 4x4     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca8x8     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca1.exr  -r 3840x2160 -s 8x8     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca10x10   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca2.exr  -r 3840x2160 -s 10x10   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca12x12   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca3.exr  -r 3840x2160 -s 12x12   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca16x16   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca4.exr  -r 3840x2160 -s 16x16   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca20x20   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca5.exr  -r 3840x2160 -s 20x20   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca24x24   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca6.exr  -r 3840x2160 -s 24x24   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca30x30   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca7.exr  -r 3840x2160 -s 30x30   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca40x40   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca8.exr  -r 3840x2160 -s 40x40   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca48x48   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca9.exr  -r 3840x2160 -s 48x48   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca60x60   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca10.exr -r 3840x2160 -s 60x60   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca80x80   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca11.exr -r 3840x2160 -s 80x80   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca120x120 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca12.exr -r 3840x2160 -s 120x120 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca240x240 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca13.exr -r 3840x2160 -s 240x240 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca480x360 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca14.exr -r 3840x2160 -s 480x360 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca768x540 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca15.exr -r 3840x2160 -s 768x540 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)
add_test(NAME cow-2160-t4-ca960x720 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca16.exr -r 3840x2160 -s 960x720 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 4 -c)

# 2160p cow with bvh 8 thread canvas size tests
add_test(NAME cow-2160-t8-ca4x4     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca0.exr  -r 3840x2160 -s 4x4     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca8x8     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca1.exr  -r 3840x2160 -s 8x8     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca10x10   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca2.exr  -r 3840x2160 -s 10x10   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca12x12   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca3.exr  -r 3840x2160 -s 12x12   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca16x16   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca4.exr  -r 3840x2160 -s 16x16   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca20x20   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca5.exr  -r 3840x2160 -s 20x20   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca24x24   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca6.exr  -r 3840x2160 -s 24x24   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca30x30   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca7.exr  -r 3840x2160 -s 30x30   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca40x40   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca8.exr  -r 3840x2160 -s 40x40   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca48x48   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca9.exr  -r 3840x2160 -s 48x48   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca60x60   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca10.exr -r 3840x2160 -s 60x60   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca80x80   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca11.exr -r 3840x2160 -s 80x80   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca120x120 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca12.exr -r 3840x2160 -s 120x120 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca240x240 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca13.exr -r 3840x2160 -s 240x240 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca480x360 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca14.exr -r 3840x2160 -s 480x360 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca768x540 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca15.exr -r 3840x2160 -s 768x540 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)
add_test(NAME cow-2160-t8-ca960x720 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca16.exr -r 3840x2160 -s 960x720 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 8 -c)

# 2160p cow with bvh 12 thread canvas size tests
add_test(NAME cow-2160-t12-ca4x4     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca0.exr  -r 3840x2160 -s 4x4     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca8x8     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca1.exr  -r 3840x2160 -s 8x8     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca10x10   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca2.exr  -r 3840x2160 -s 10x10   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca12x12   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca3.exr  -r 3840x2160 -s 12x12   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca16x16   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca4.exr  -r 3840x2160 -s 16x16   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca20x20   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca5.exr  -r 3840x2160 -s 20x20   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca24x24   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca6.exr  -r 3840x2160 -s 24x24   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca30x30   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca7.exr  -r 3840x2160 -s 30x30   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca40x40   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca8.exr  -r 3840x2160 -s 40x40   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca48x48   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca9.exr  -r 3840x2160 -s 48x48   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca60x60   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca10.exr -r 3840x2160 -s 60x60   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca80x80   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca11.exr -r 3840x2160 -s 80x80   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca120x120 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca12.exr -r 3840x2160 -s 120x120 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca240x240 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca13.exr -r 3840x2160 -s 240x240 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca480x360 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca14.exr -r 3840x2160 -s 480x360 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca768x540 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca15.exr -r 3840x2160 -s 768x540 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)
add_test(NAME cow-2160-t12-ca960x720 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca16.exr -r 3840x2160 -s 960x720 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 12 -c)

# 2160p cow with bvh 16 thread canvas size tests
add_test(NAME cow-2160-t16-ca4x4     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca0.exr  -r 3840x2160 -s 4x4     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca8x8     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca1.exr  -r 3840x2160 -s 8x8     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca10x10   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca2.exr  -r 3840x2160 -s 10x10   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca12x12   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca3.exr  -r 3840x2160 -s 12x12   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca16x16   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca4.exr  -r 3840x2160 -s 16x16   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca20x20   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca5.exr  -r 3840x2160 -s 20x20   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca24x24   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca6.exr  -r 3840x2160 -s 24x24   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca30x30   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca7.exr  -r 3840x2160 -s 30x30   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca40x40   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca8.exr  -r 3840x2160 -s 40x40   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca48x48   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca9.exr  -r 3840x2160 -s 48x48   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca60x60   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca10.exr -r 3840x2160 -s 60x60   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca80x80   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca11.exr -r 3840x2160 -s 80x80   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca120x120 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca12.exr -r 3840x2160 -s 120x120 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca240x240 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca13.exr -r 3840x2160 -s 240x240 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca480x360 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca14.exr -r 3840x2160 -s 480x360 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca768x540 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca15.exr -r 3840x2160 -s 768x540 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)
add_test(NAME cow-2160-t16-ca960x720 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca16.exr -r 3840x2160 -s 960x720 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 16 -c)

# 2160p cow with bvh 20 thread canvas size tests
add_test(NAME cow-2160-t20-ca4x4     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca0.exr  -r 3840x2160 -s 4x4     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca8x8     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca1.exr  -r 3840x2160 -s 8x8     -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca10x10   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca2.exr  -r 3840x2160 -s 10x10   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca12x12   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca3.exr  -r 3840x2160 -s 12x12   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca16x16   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca4.exr  -r 3840x2160 -s 16x16   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca20x20   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca5.exr  -r 3840x2160 -s 20x20   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca24x24   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca6.exr  -r 3840x2160 -s 24x24   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca30x30   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca7.exr  -r 3840x2160 -s 30x30   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca40x40   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca8.exr  -r 3840x2160 -s 40x40   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca48x48   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca9.exr  -r 3840x2160 -s 48x48   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca60x60   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca10.exr -r 3840x2160 -s 60x60   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca80x80   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca11.exr -r 3840x2160 -s 80x80   -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca120x120 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca12.exr -r 3840x2160 -s 120x120 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca240x240 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca13.exr -r 3840x2160 -s 240x240 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca480x360 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca14.exr -r 3840x2160 -s 480x360 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca768x540 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca15.exr -r 3840x2160 -s 768x540 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)
add_test(NAME cow-2160-t20-ca960x720 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-2160-ca16.exr -r 3840x2160 -s 960x720 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj -b -t 20 -c)































#add_test(NAME cow              COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-cow-output.exr              -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj)
#add_test(NAME igea             COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-head-output.exr             -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj)
#add_test(NAME homer            COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-homer-output.exr            -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/homer.obj)
#add_test(NAME lucy             COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-lucy-output.exr             -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/lucy.obj)
#add_test(NAME max-planck       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-max-planck-output.exr       -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/max-planck.obj)
#add_test(NAME nefertiti        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-nefertiti-output.exr        -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/nefertiti.obj)
#add_test(NAME ogre             COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-ogre-output.exr             -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/ogre.obj)
#add_test(NAME spot             COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-spot-output.exr             -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/spot.obj)
#add_test(NAME stanford-bunny   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-stanford-bunny-output.exr   -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/stanford-bunny.obj)
#add_test(NAME suzanne          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-suzanne-output.exr          -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/suzanne.obj)
#add_test(NAME teapot           COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-output.exr           -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj)
#add_test(NAME xyzrgb_dragon    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-xyzrgb_dragon-output.exr    -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/xyzrgb_dragon.obj)
# 
#add_test(NAME cow4k            COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kcow-output.exr            -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/cow.obj)
#add_test(NAME homer4k          COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4khomer-output.exr          -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/homer.obj)
#add_test(NAME igea4k           COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4khead-output.exr           -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj)
#add_test(NAME lucy4k           COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4klucy-output.exr           -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/lucy.obj)
#add_test(NAME max-planck4k     COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kmax-planck-output.exr     -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/max-planck.obj)
#add_test(NAME nefertiti4k      COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4knefertiti-output.exr      -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/nefertiti.obj)
#add_test(NAME ogre4k           COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kogre-output.exr           -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/ogre.obj)
#add_test(NAME spot4k           COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kspot-output.exr           -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/spot.obj)
#add_test(NAME stanford-bunny4k COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kstanford-bunny-output.exr -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/stanford-bunny.obj)
#add_test(NAME suzanne4k        COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4ksuzanne-output.exr        -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/suzanne.obj)
#add_test(NAME teapot4k         COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kteapot-output.exr         -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj)
#add_test(NAME xyzrgb_dragon4k  COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-4kxyzrgb_dragon-output.exr  -r 3840x2160 -i ${CMAKE_SOURCE_DIR}/obj/xyzrgb_dragon.obj)