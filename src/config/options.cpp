#include "options.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <unistd.h>


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

    // Parse command line args
    for (int c; (c = getopt(argc, argv, "r:i:o:")) != -1;)
    {
        switch (c)
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
            default:
            {
                assert(false);
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