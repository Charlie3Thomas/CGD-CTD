#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

int main()
{

    // I have some weightings for different parameters at different values



    std::cout << "OPTM Tester" << std::endl;

    std::string ray_tracer = "./build/release/apps/ray-tracer";

    std::string cmd = ray_tracer + " -o output/oingo.exr -r 1920x1080 -d 1 -h 8 -i 3 -k -e 1 | tee log.txt";

    int result = std::system(cmd.c_str());

    if (result != 0)
    {
        std::cerr << "Oopsie woopsie, we did a widdwl fucky wucky! Ewwow executing the command. Pwease twy again >w<. ";
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}