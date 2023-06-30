#include "utils.hpp"

#include <atomic>
#include <random>

namespace CT
{
uint64_t GetGUID()
{
    static std::atomic<uint64_t> guid{0};
    return guid++;
}

float RandomRange(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);    
}

float RandomNormalDistribution()
{
    auto theta = static_cast<float>(2.0F * M_PI * RandomRange(0.0F, 1.0F));
    float rho = std::sqrt(-2.0F * std::log(RandomRange(0.0F, 1.0F)));
    return rho * std::cos(theta);
}

// http://psgraphics.blogspot.com/2011/01/improved-code-for-concentric-map.html
void ToUnitDisk(double seedx, double seedy, double *x, double *y)
{
    double phi = 0;
    double r = 0;

    double a = 2.0F * seedx - 1.0F;
    double b = 2.0F * seedy - 1.0F;

    if (a > -b) 
    {
        if (a > b)
        {
            r = a;
            phi = (M_PI / 4.0F) * (b / a);
        }
        else
        {
            phi = (M_PI / 4.0F) * (2.0F - (a / b));
        }
    }
    else 
    {
        if (a < b) 
        {
            r = -a;
            phi = (M_PI / 4.0F) * (4.0F + (b / a));
        }
        else
        {
            r = -b;
            if (b !=0){
                phi = (M_PI / 4.0F) * (6.0F - (a / b));
            }
            else{
                phi = 0.0F;
            }
        }
    }

    *x = r * cos(phi);
    *y = r * sin(phi);    
}

}
