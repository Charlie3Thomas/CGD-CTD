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

# 4320p teapot no bvh thread tests
add_test(NAME teapot-4320-t01-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 1 )
add_test(NAME teapot-4320-t02-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 2 )
add_test(NAME teapot-4320-t04-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 4 )
add_test(NAME teapot-4320-t08-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 8 )
add_test(NAME teapot-4320-t12-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 12)
add_test(NAME teapot-4320-t16-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 16)
add_test(NAME teapot-4320-t20-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320.exr      -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj    -t 20)
# 4320p teapot  
add_test(NAME teapot-4320-t01-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 1 )
add_test(NAME teapot-4320-t02-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 2 )
add_test(NAME teapot-4320-t04-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 4 )
add_test(NAME teapot-4320-t08-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 8 )
add_test(NAME teapot-4320-t12-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 12)
add_test(NAME teapot-4320-t16-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 16)
add_test(NAME teapot-4320-t20-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-teapot-4320-b.exr    -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/teapot.obj -b -t 20)

# 4320p igea no bvh thread tests
add_test(NAME igea-4320-t01-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 1 )
add_test(NAME igea-4320-t02-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 2 )
add_test(NAME igea-4320-t04-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 4 )
add_test(NAME igea-4320-t08-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 8 )
add_test(NAME igea-4320-t12-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 12)
add_test(NAME igea-4320-t16-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 16)
add_test(NAME igea-4320-t20-b0c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320.exr          -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj    -t 20)
# 4320p igea    
add_test(NAME igea-4320-t01-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 1 )
add_test(NAME igea-4320-t02-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 2 )
add_test(NAME igea-4320-t04-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 4 )
add_test(NAME igea-4320-t08-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 8 )
add_test(NAME igea-4320-t12-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 12)
add_test(NAME igea-4320-t16-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 16)
add_test(NAME igea-4320-t20-b1c0n0 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-igea-4320-b.exr        -r 7680x4320 -i ${CMAKE_SOURCE_DIR}/obj/igea.obj -b -t 20)



add_executable(ct-test-textureloader testtextureload.cpp)
target_link_libraries(ct-test-textureloader PRIVATE freeimage ct-embree)

#add_executable(ct-test-boingo boingo.cpp)