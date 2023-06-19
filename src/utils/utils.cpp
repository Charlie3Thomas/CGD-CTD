#include "utils.hpp"

#include <atomic>

uint64_t GetGUID()
{
    static std::atomic<uint64_t> guid{0};
    return guid++;
}