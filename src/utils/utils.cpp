#include "utils.hpp"

#include <tinyexr.h>

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
    // static std::random_device rd;
    // static std::mt19937 gen(rd());
    static std::mt19937 gen(42069);
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

float* LoadEXRFromFile(const char* filename, int& width, int& height)
{
    float* rgba = nullptr;
    const char* err;

    int ret = LoadEXR(&rgba, &width, &height, filename, &err);

    if (ret != TINYEXR_SUCCESS) 
    {
        std::cerr << "Error loading EXR: " << err << std::endl;
        FreeEXRErrorMessage(err);
        std::throw_with_nested(std::runtime_error("Error loading EXR"));
    }

    return rgba;
}

float L1Difference(const float* a, const float* b, size_t height, size_t width)
{
    float L1_sum_a = 0.0F;
    float L1_sum_b = 0.0F;

    for (size_t y = 0; y < static_cast<size_t>(height); y++)
    {
        for (size_t x = 0; x < static_cast<size_t>(width); x++)
        {
            float Ra = a[4 * (y * width + x) + 0];
            float Ga = a[4 * (y * width + x) + 1];
            float Ba = a[4 * (y * width + x) + 2];
            L1_sum_a += (Ra + Ga + Ba) / 3.0F;

            float Rb = b[3 * (y * width + x) + 0];
            float Gb = b[3 * (y * width + x) + 1];
            float Bb = b[3 * (y * width + x) + 2];
            L1_sum_b += (Rb + Gb + Bb) / 3.0F;
        }
    }

    L1_sum_a /= static_cast<float>(width * height);
    L1_sum_b /= static_cast<float>(width * height);
    float L1_diff = std::abs(L1_sum_a - L1_sum_b);

    return L1_diff;
}

float L2Difference(const float* a, const float* b, size_t height, size_t width)
{
    float L2_sum = 0.0F;

    for (size_t y = 0; y < static_cast<size_t>(height); y++)
    {
        for (size_t x = 0; x < static_cast<size_t>(width); x++)
        {
            float Ra = a[4 * (y * width + x) + 0];
            float Ga = a[4 * (y * width + x) + 1];
            float Ba = a[4 * (y * width + x) + 2];

            float Rb = b[3 * (y * width + x) + 0];
            float Gb = b[3 * (y * width + x) + 1];
            float Bb = b[3 * (y * width + x) + 2];

            float L2_R = std::pow(Ra - Rb, 2.0F);
            float L2_G = std::pow(Ga - Gb, 2.0F);
            float L2_B = std::pow(Ba - Bb, 2.0F);
            L2_sum += (L2_R + L2_G + L2_B) / 3.0F;
        }
    }

    L2_sum /= static_cast<float>(width * height);

    return L2_sum;
}

}
