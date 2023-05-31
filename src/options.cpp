#include "options.hpp"

#include "scene/test_scenes.hpp"

#include <cstring>
#include <stdexcept>

#include <unistd.h>

options parse_options(int argc, char** argv)
{
    options ret;

    for (int c; (c = getopt(argc, argv, "f:r:o:s:")) != -1; )
    {
        switch (c)
        {
            // Output file format
            case 'f':
            {
                if (strcmp(optarg, "png") == 0)
                    ret.format = options::format_t::png;
                else if (strcmp(optarg, "ppm") == 0)
                    ret.format = options::format_t::ppm;
                else
                    throw std::runtime_error("Invalid file format: " + std::string(optarg));
                break;
            }
            // Resolution
            case 'r':
            {
                const std::string resolution = std::string(optarg);
                const std::size_t xpos = resolution.find('x');
                ret.horizontal_pixels = std::stoi(resolution.substr(0, xpos));
                ret.vertical_pixels  = std::stoi(resolution.substr(xpos + 1, resolution.length()));
                break;
            }
            // Output file name
            case 'o':
            {
                ret.output_file = optarg;
                break;
            }
            // Test scene
            case 's':
            {
                if (strcmp(optarg, "sphere") == 0)
                    ret.test_scene = &scene::single_sphere;
                else if (strcmp(optarg, "cylinders") == 0)
                    ret.test_scene = &scene::cylinders;
                else
                    throw std::runtime_error("Unknown test scene: " + std::string(optarg));
                break;
            }
            // Unknown
            case '?':
            {
                if (optopt == 'f' || optopt == 'r' || optopt == 'o')
                    throw std::runtime_error("Option requires an argument");
                throw std::runtime_error("Unknown option");
                break;
            }
            default:
                throw std::runtime_error("Could not parse commandline options");   
        }
    }

    return ret;
}