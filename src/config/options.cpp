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

    const char* const short_opts = "r:i:o:t:bcn";
    const option long_opts[] = {
        {"resolution", required_argument, nullptr, 'r'},
        {"input",      required_argument, nullptr, 'i'},
        {"output",     required_argument, nullptr, 'o'},
        {"threads",    required_argument, nullptr, 't'},
        {"bvh",        no_argument,       nullptr, 'b'},
        {"canvases",   no_argument,       nullptr, 'c'},
        {"normals",    no_argument,       nullptr, 'n'},
        {nullptr,      no_argument,       nullptr,  0}
    };

    // Parse command line args
    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
            case 'r':
            {
                // Parse resolution
                const std::string resolution = optarg;
                const std::size_t x_pos = resolution.find('x');
                instance->image_width = std::stol(resolution.substr(0, x_pos));
                instance->image_height = std::stol(resolution.substr(x_pos + 1, resolution.length()));
                break;
            }
            case 'i':
            {
                instance->input_model_filename = optarg;
                break;
            }
            case 'o':
            {
                instance->image_filename = optarg;
                break;
            }
            case 't':
            {
                instance->threads = std::stol(optarg);
                break;
            }
            case 'b':
            {
                instance->use_bvh = true;                
                std::cout << "Using BVH" << std::endl;
                break;
            }
            case 'c':
            {
                instance->visualise_canvases = true;
                std::cout << "Visualising canvases" << std::endl;
                break;
            }
            case 'n':
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

    // // Parse command line args
    // for (int c; (c = getopt(argc, argv, "r:i:o:b:c:n")) != -1;)
    // {
    //     switch (c)
    //     {
    //         case 'r':
    //         {
    //             // Parse resolution
    //             const std::string resolution = optarg;
    //             const std::size_t x_pos = resolution.find('x');
    //             instance->image_width = std::stol(resolution.substr(0, x_pos));
    //             instance->image_height = std::stol(resolution.substr(x_pos + 1, resolution.length()));
    //             break;
    //         }
    //         case 'i':
    //         {
    //             instance->input_model_filename = optarg;
    //             break;
    //         }
    //         case 'o':
    //         {
    //             instance->image_filename = optarg;
    //             break;
    //         }
    //         case 'b':
    //         {
    //             instance->use_bvh = true;                
    //             break;
    //         }
    //         case 'c':
    //         {
    //             instance->visualise_canvases = true;
    //             break;
    //         }
    //         case 'n':
    //         {
    //             instance->visualise_normals = true;
    //             break;
    //         }
    //         default:
    //         {
    //             assert(false);
    //             break;
    //         }
    //     }        
    // }
}

ConfigSingleton::~ConfigSingleton()
{
    delete instance;
}
}