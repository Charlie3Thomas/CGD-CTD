#include "utils.hpp"

#include <atomic>
#include <random>
#include <iostream>
#include <map>
#include <iomanip>

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

// float RandomNormalDistribution()
// {
//     std::random_device rd{};
//     std::mt19937 gen{ rd() };

//     // values near the mean are the most likely
//     // standard deviation affects the dispersion of generated values from the mean
//     std::normal_distribution<> d{ 5, 2 };

//     std::map<int, int> hist{};
//     for (int n = 0; n != 10000; ++n)
//         ++hist[std::round(d(gen))];

//     for (auto [x, y] : hist)
//         std::cout << std::setw(2) << x << ' ' << std::string(y / 200, '*') << '\n';
// }

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
