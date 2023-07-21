#include "options.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <unistd.h>
#include <getopt.h>
#include <iostream>


namespace CT
{

static ConfigSingleton* instance = nullptr;

ConfigSingleton& ConfigSingleton::GetInstance()
{
    assert(instance != nullptr);
    return *instance;
}

void ConfigSingleton::ParseOptions(int argc, char** argv)
{
    assert(instance == nullptr);

    instance = new ConfigSingleton();

    const char* const short_opts = "r:e:o:s:d:h:i:kbcn"; 
    const option long_opts[] = {
        {"resolution",       required_argument, nullptr, 'r'},
        {"environment",      required_argument, nullptr, 'e'},
        {"output",           required_argument, nullptr, 'o'},
        {"segments",         required_argument, nullptr, 's'},
        {"direct_samples",   required_argument, nullptr, 'd'},
        {"indirect_samples", required_argument, nullptr, 'h'},
        {"recursion_depth",  required_argument, nullptr, 'i'},
        {"denoiser",         no_argument,       nullptr, 'k'},
        {"bvh",              no_argument,       nullptr, 'b'},
        {"canvases",         no_argument,       nullptr, 'c'},
        {"normals",          no_argument,       nullptr, 'n'},
        {nullptr,            no_argument,       nullptr,  0 }
    };

    // Parse command line args
    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
            case 'r': // --resolution
            {
                // Parse resolution
                const std::string resolution = optarg;
                const std::size_t x_pos      = resolution.find('x');
                instance->image_width        = std::stol(resolution.substr(0, x_pos));
                instance->image_height       = std::stol(resolution.substr(x_pos + 1, resolution.length()));
                break;
            }
            case 'e': // --environment
            {
                // Parse environment
                size_t env = std::stol(optarg);
                switch (env)
                {
                    case 1:
                    {
                        instance->environment = double_dragon;
                        break;
                    }
                    case 2:
                    {
                        instance->environment = triple_statue;
                        break;
                    }
                    case 3:
                    {
                        instance->environment = cornell_box;
                        break;
                    }
                    case 4:
                    {
                        instance->environment = triple_statue_area_light;
                        break;
                    }
                }
                break;
            }
            case 'o': // --output filename
            {
                instance->image_filename = optarg;
                break;
            }
            case 's': // --segments
            {
                // Parse canvas resolution
                const std::string canvas = optarg;
                const std::size_t x_pos  = canvas.find('x');
                instance->canvas_width   = std::stol(canvas.substr(0, x_pos));
                instance->canvas_height  = std::stol(canvas.substr(x_pos + 1, canvas.length()));
            }
            case 'd': // --direct_samples
            {
                instance->direct_samples = std::stol(optarg);
                break;
            }
            case 'h': // --indirect_samples
            {
                instance->indirect_samples = std::stol(optarg);
                break;
            }
            case 'i': // --recursion_depth
            {
                instance->recursion_depth = std::stol(optarg);
                break;
            }
            case 'k': // --denoiser
            {
                instance->denoiser = true;
                std::cout << "Using denoiser" << std::endl;
                break;
            }
            case 'b': // --bvh
            {
                instance->use_bvh = true;                
                std::cout << "Using BVH" << std::endl;
                break;
            }
            case 'c': // --canvases
            {
                instance->visualise_canvases = true;
                std::cout << "Visualising canvases" << std::endl;
                break;
            }
            case 'n': // --normals
            {
                instance->visualise_normals = true;
                std::cout << "Visualising normals" << std::endl;
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

ConfigSingleton::~ConfigSingleton()
{
    delete instance;
}
}