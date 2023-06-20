#TODO : More sensible test structure

add_test(NAME guy-1080 COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-guy-1080.exr -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/guy.obj -t 20)

# List of model file names
set(objfiles
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

# Iterate over the models
foreach(model ${objfiles})
    foreach(i RANGE 1 2)
        math(EXPR t_value "2 * ${i}")
        if(${i} MATCHES "[02468]")
            set(ray_tracer_flags "-b")
            set(output_suffix "-b")
        else()
            set(ray_tracer_flags "")
            set(output_suffix "")
        endif()
        add_test(
            NAME ${model}-1080-${i}
            COMMAND ray-tracer -o ${CMAKE_SOURCE_DIR}/output/test-${model}-1080-${output_suffix}-${i}.exr -r 1920x1080 -i ${CMAKE_SOURCE_DIR}/obj/${model}.obj ${ray_tracer_flags} -t 20
        )
    endforeach()
endforeach()