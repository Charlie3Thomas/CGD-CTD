add_test(NAME drag       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 1) 
add_test(NAME drag-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 1 -n)
add_test(NAME stat       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 2) 
add_test(NAME stat-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 2 -n)
add_test(NAME corn       COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 64 -i 3 -k -e 3) 
add_test(NAME corn-norms COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 3 -n)
add_test(NAME ob         COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 4) 
add_test(NAME ob-norms   COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-test.exr -h 8 -i 3 -k -e 4 -n)
#TODO : More sensible test structure


# List of model file names
set(objfiles
capsule
cow
cube
homer
igea
lucy
max-planck
nefertiti
ogre
plant
spot
stanford-bunny
suzanne
teapot
triangle
xyzrgb_dragon
)

## Iterate over the models
#foreach(model ${objfiles})
#foreach(i RANGE 1 2)
#math(EXPR t_value "2 * ${i}")
#if(${i} MATCHES "[02468]")
#set(ray_tracer_flags "-b")
#set(output_suffix "-b")
#else()
#set(ray_tracer_flags "")
#set(output_suffix "")
#endif()
#add_test(
#    NAME ${model}-1080${output_suffix}
#    COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-${model}-1080-${output_suffix}-${i}.exr -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/${model}.obj ${ray_tracer_flags} -t 20
#    )
#    endforeach()
#endforeach()
