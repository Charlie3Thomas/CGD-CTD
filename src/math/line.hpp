#pragma once

#include "vector.hpp"

namespace math
{

template <typename T, std::size_t S>
struct line
{
    vector<T, S> operator[](const T& t) const { return origin + t*direction; }

    vector<T, S> origin;
    vector<T, S> direction;
};

}