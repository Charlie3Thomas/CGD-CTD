#pragma once

#include <exception>
#include <cstddef>
#include <cstdint>

namespace math
{

template <typename T>
constexpr T kronecker_delta(std::size_t i, std::size_t j)
{
    return static_cast<T>(i == j);
}

template <typename T>
constexpr T levi_civita(std::size_t i, std::size_t j, std::size_t k)
{
    if (i > 2 || j > 2 || k > 2)
        throw std::runtime_error("Invalid range");

    return static_cast<T>(((static_cast<std::int64_t>(i) - static_cast<std::int64_t>(j))
        * (static_cast<std::int64_t>(j) - static_cast<std::int64_t>(k))
        * (static_cast<std::int64_t>(k) - static_cast<std::int64_t>(i))) / 2);
}

}