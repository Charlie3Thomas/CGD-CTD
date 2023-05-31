#pragma once

#include <cmath>
#include <array>
#include <iostream>
#include <concepts>

namespace math
{

/**
 * @brief An arithmetic vector type
 * This objects publicly inherits from std::array so cannot contain any state of its own (otherwise we
 * would need std::array to have a virtual destructor)
 * @tparam T An integral type 
 * @tparam S The dimension of the vector
 */
template <typename T, std::size_t S>
struct vector : std::array<T, S>
{
    // Equivalence overloads
    constexpr bool operator==(const vector& v) const noexcept { return std::equal(this->begin(), this->end(), v.begin()); }
    constexpr bool operator!=(const vector& v) const noexcept { return !(*this == v); }

    // General arithmetic overloads
    constexpr const vector& operator+() const noexcept { return *this; }
    constexpr vector operator-() const noexcept { return T(- 1) * (*this); }
    constexpr vector& operator+=(const vector& v) noexcept;
    constexpr vector& operator-=(const vector& v) noexcept;
    constexpr vector& operator*=(const auto& t) noexcept;
    constexpr vector& operator/=(const auto& t) noexcept;

    constexpr vector& normalise() noexcept;
};

// Non-member arithmetic overloads
template <typename T, std::size_t S>
constexpr vector<T, S> operator+(const vector<T, S>& v, const vector<T, S>& w)
{
    vector<T, S> ret;
    for (std::size_t i = 0; i < S; i++) 
        ret[i] = v[i] + w[i];
    return ret;
}

template <typename T, std::size_t S>
constexpr vector<T, S> operator-(const vector<T, S>& v, const vector<T, S>& w)
{
    vector<T, S> ret;
    for (std::size_t i = 0; i < S; i++) 
        ret[i] = v[i] - w[i]; 
    return ret;
}

template <typename T, std::size_t S>
constexpr vector<T, S> operator*(const vector<T, S>& v, const auto& t)
{
    vector<T, S> ret;
    for (std::size_t i = 0; i < S; i++)
        if constexpr (std::is_integral<T>::value)
            ret[i] = std::round(v[i] * t);
        else
            ret[i] = v[i] * t;
    return ret;
}

template <typename T, std::size_t S>
constexpr vector<T, S> operator*(const auto& t, const vector<T, S>& v)
{
    return v * t;
}

template <typename T, std::size_t S>
constexpr vector<T, S> operator*(const vector<T, S>& v, const vector<T, S>& w)
{
    vector<T, S> ret;;
    for (std::size_t i = 0; i < S; i++)
        ret[i] = v[i] + w[i];
    return ret;
}

template <typename T, std::size_t S, typename U>
constexpr vector<T, S> operator/(const vector<T, S>& v, const U& t)
{
    if constexpr (std::is_floating_point<U>::value)
        return v * (1 / t);
    else
        return v / static_cast<float>(t);
}

// Dot, cross, and triple product
template <typename T, std::size_t S>
constexpr T dot_product(const vector<T, S>& v, const vector<T, S>& w)
{
    T ret = 0;
    for (std::size_t i = 0; i < S; i++) ret += v[i] * w[i];
    return ret;
}

template <typename T, std::size_t S>
requires (S == 3)
constexpr vector<T, S> cross_product(const vector<T, S>& v, const vector<T, S>& w)
{
    return { v[1] * w[2] - v[2] * w[1], v[2] * w[0] - v[0] * w[2], v[0] * w[1] - v[1] * w[0] };
}

template <typename T, std::size_t S>
constexpr T scalar_triple_product(const vector<T, S>& v, const vector<T, S>& w, const vector<T, S>& u)
{
    return dot_product(v, cross_product(w, u));
}

/**
 * @brief Calculates the square Euclidean length of an STL container
 */
template <typename T>
typename T::value_type square_length(const T& v) noexcept
{
    typename T::value_type ret = 0;
    for (const auto& i : v)
        ret += i*i;
    return ret;
}

/**
 * @brief Calculates the Euclidean length of an STL container of floating point types
 */
template <typename T>
requires std::floating_point<typename T::value_type>
typename T::value_type length(const T& v) noexcept
{
    return std::sqrt(square_length(v));
}

/**
 * @brief Calculates the inverse square Euclidean length of an STL container of floating point types
 */
template <typename T>
requires std::floating_point<typename T::value_type>
typename T::value_type inverse_square_length(const T& v) noexcept
{
    return 1/square_length(v);
}

/**
 * @brief Calculates the inverse Euclidean length of an STL container of floating point types
 * We tried using the old Quake II version of this algorithm and it was no faster (but significantly less safe) than
 * this one here.
 */
template <typename T>
requires std::floating_point<typename T::value_type>
typename T::value_type inverse_length(const T& v) noexcept
{
    return 1/length(v);
}

/**
 * @brief Normalises the vector
 */
auto normalise(const auto& v)
{
    return v * inverse_length(v);
}

// Ostream overload
template <typename T, std::size_t S>
std::ostream& operator<<(std::ostream& os, const vector<T, S>& v)
{
    for (const auto& i : v)
        if constexpr (std::is_integral<T>::value)
            os << static_cast<int>(i) << ' ';
        else
            os << i << ' ';
    return os;
}

// Member arithmetic overloads
template <typename T, std::size_t S>
constexpr vector<T, S>& vector<T, S>::operator+=(const vector<T, S>& t) noexcept
{
    return (*this) = (*this)+t;
}

template <typename T, std::size_t S>
constexpr vector<T, S>& vector<T, S>::operator-=(const vector<T, S>& t) noexcept
{
    return (*this) = (*this)-t;
}

template <typename T, std::size_t S>
constexpr vector<T, S>& vector<T, S>::operator*=(const auto& t) noexcept
{
    return (*this) = (*this)*t;
}

template <typename T, std::size_t S>
constexpr vector<T, S>& vector<T, S>::operator/=(const auto& t) noexcept
{
    return (*this) = (*this)/t;
}

template <typename T, std::size_t S>
constexpr vector<T, S>& vector<T, S>::normalise() noexcept
{
    return (*this) = math::normalise(*this);
}

}
