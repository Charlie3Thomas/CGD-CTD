#pragma once

#include <concepts>

namespace math
{

template <typename T>
requires std::unsigned_integral<T>
constexpr auto pow(const auto& a, const T& b)
{
    decltype(a) ret = 1;
    for (std::size_t i = 0; i < b; i++)
        ret *= a;
    return ret;
}

}