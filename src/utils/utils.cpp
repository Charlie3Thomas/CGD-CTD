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

// template<typename T>
// T RandomRange(T min, T max)
// {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     std::uniform_real_distribution<T> dis(min, max);
//     return dis(gen);    
// }

float RandomRange(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);    
}
}
