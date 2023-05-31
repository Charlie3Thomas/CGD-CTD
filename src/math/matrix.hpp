#pragma once

#include "vector.hpp"
#include "tensor.hpp"

namespace math
{

template <typename T, std::size_t M, std::size_t N>
struct matrix : std::array<T, M*N>
{
    // Quality-of-life overloads
    constexpr bool operator==(const matrix& A) const { return std::equal(this->begin(), this->end(), A.begin()); }
    constexpr bool operator!=(const matrix& A) const { return !(*this == A); }

    // Index operators (unfortunately we cannot overload [] because this needs exactly one argument)
    constexpr T& operator()(std::size_t m, std::size_t n) { return (*this)[m*N + n]; }
    constexpr const T& operator()(std::size_t m, std::size_t n) const { return (*this)[m*N + n]; }

    // Arithmetic overloads
    constexpr const matrix& operator+() const     noexcept { return *this; }
    constexpr matrix operator-() const            noexcept { return T(- 1) * (*this); }
    constexpr matrix& operator+=(const matrix& A) noexcept;
    constexpr matrix& operator-=(const matrix& A) noexcept;
    constexpr matrix& operator*=(const auto& t)   noexcept;
    constexpr matrix& operator/=(const auto& t)   noexcept;
};

template <typename T, std::size_t M>
constexpr matrix<T, M, M> identity()
{
    matrix<T, M, M> ret { 0 };
    for (std::size_t i = 0; i < M; i++)
        for (std::size_t j = 0; j < M; j++)
            ret(i, j) = kronecker_delta<T>(i, j);

    return ret;
}

// Non-member arithmetic overloads
template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N> operator+(const matrix<T, M, N>& A, const matrix<T, M, N>& w)
{
    matrix<T, M, N> ret;
    for (std::size_t i = 0; i < M*N; i++) ret[i] = A[i] + w[i];
    return ret;
}

template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N> operator-(const matrix<T, M, N>& A, const matrix<T, M, N>& w)
{
    matrix<T, M, N> ret;
    for (std::size_t i = 0; i < M*N; i++) ret[i] = A[i] - w[i];
    return ret;
}

template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N> operator*(const matrix<T, M, N>& A, const auto& t)
{
    matrix<T, M, N> ret;
    for (std::size_t i = 0; i < M*N; i++) 
        ret[i] = A[i] * t;
    return ret;
}

template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N> operator*(const auto& t, const matrix<T, M, N>& A)
{
    return A * t;
}

template <typename T, std::size_t M, std::size_t N, std::size_t O>
constexpr matrix<T, M, O> operator*(const matrix<T, M, N>& A, const matrix<T, N, O>& w)
{
    matrix<T, M, O> ret = { 0 };
    for (std::size_t i = 0; i < M; i++) 
        for (std::size_t j = 0; j < N; j++) 
            for (std::size_t k = 0; k < O; k++) 
                ret(i, k) += A(i, j) * w(j, k);
    return ret;
}

template <typename T, std::size_t M, std::size_t N>
constexpr vector<T, M> operator*(const matrix<T, M, N>& A, const vector<T, N>& v)
{
    vector<T, N> ret = { 0 };
    for (std::size_t i = 0; i < N; i++) 
        for (std::size_t j = 0; j < M; j++) 
            ret[i] += A(i, j)*v[j];
    return ret;
}

template <typename T, std::size_t M, std::size_t N, typename S>
constexpr matrix<T, M, N> operator/(const matrix<T, M, N>& A, const S& t)
{
    if constexpr (std::is_floating_point<S>::value)
        return A * (1 / t);
    else
        return A / static_cast<float>(t);
}

// Member arithmetic overloads
template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N>& matrix<T, M, N>::operator+=(const matrix<T, M, N>& t) noexcept
{
    return (*this)+t;
}

template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N>& matrix<T, M, N>::operator-=(const matrix<T, M, N>& t) noexcept
{
    return (*this)-t;
}

template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N>& matrix<T, M, N>::operator*=(const auto& t) noexcept
{
    return (*this) = (*this)*t;
}

template <typename T, std::size_t M, std::size_t N>
constexpr matrix<T, M, N>& matrix<T, M, N>::operator/=(const auto& t) noexcept
{
    return (*this) = (*this)/t;
}

/**
 * @brief Generates the rotation matrix from the Euler angles
 */
template <typename T>
requires std::floating_point<T>
matrix<T, 3, 3> rotation(T a, T b, T c)
{
    using namespace std;
    return { cos(b)*cos(c), sin(a)*sin(b)*cos(c) - cos(a)*sin(c), cos(a)*sin(b)*cos(c) + sin(a)*sin(c),
             cos(b)*sin(c), sin(a)*sin(b)*sin(c) + cos(a)*cos(c), cos(a)*sin(b)*sin(c) - sin(a)*cos(c),
            -sin(b),        sin(a)*cos(b),                        cos(a)*cos(b) };
}

/**
 * @brief Calculates the determinant
 * We only support 2x2 and 3x3 matrices at the moment
 */
template <typename T, std::size_t M>
requires std::floating_point<T> && (M == 3 || M == 2)
constexpr T determinant(const matrix<T, M, M>& A)
{
    T ret = 0;

    if constexpr (M == 2)
        ret = A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0);
    else
        for (std::size_t i = 0; i < 3; i++)
            for (std::size_t j = 0; j < 3; j++)
                for (std::size_t k = 0; k < 3; k++)
                    ret += levi_civita<T>(i, j, k) * A(0, i) * A(1, j) * A(2, k);    

    return ret;
}

/**
 * @brief Calculates the trace
 */
template <typename T, std::size_t M>
constexpr T trace(const matrix<T, M, M>& A)
{
    T ret = 0;
    for (std::size_t i = 0; i < M; i++)
        ret += A(i, i);
    return ret;
}

/**
 * @brief Calculates the norm of the matrix
 */
template <typename T, std::size_t M, std::size_t N>
constexpr T norm(const matrix<T, M, N>& A)
{
    T ret = 0;
    for (std::size_t i = 0; i < M; i++)
        for (std::size_t j = 0; j < N; j++)
            ret += std::pow(A(i, j), 2);
    return std::sqrt(ret);
}

/**
 * @brief Calculates the transpose
 */
template <typename T, std::size_t M>
constexpr matrix<T, M, M> transpose(const matrix<T, M, M>& A)
{
    matrix<T, M, M> ret;
    for (std::size_t i = 0; i < M; i++)
        for (std::size_t j = 0; j < M; j++)
            ret(i, j) = A(j, i);
    return ret;
}

/**
 * @brief Calculates the inverse
 * We only support 3x3 matrices at the moment
 */
template <typename T, std::size_t M>
requires std::floating_point<T> && (M == 3)
constexpr matrix<T, M, M> invert(const matrix<T, M, M>& A)
{
    // Find the determinant of the matrix
    const auto det = determinant(A);
    if (std::abs(det) < std::numeric_limits<double>::epsilon())
        throw std::runtime_error("Matrix is singular");

    // Find the matrix of cofactors
    matrix<T, M, M> cofactors;
    for (std::size_t i = 0; i < 3; i++)
        for (std::size_t j = 0; j < 3; j++)
            cofactors(i, j) = determinant(matrix<T, 2, 2>{
                A((i + 1) % 3, (j + 1) % 3), A((i + 1) % 3, (j + 2) % 3),
                A((i + 2) % 3, (j + 1) % 3), A((i + 2) % 3, (j + 2) % 3)
            });

    // Divide through the transpose by the determinant and return
    return transpose(cofactors) / det;
}

//  Ostream overload
template <typename T, std::size_t M, std::size_t N>
std::ostream& operator<<(std::ostream& os, const matrix<T, M, N>& A)
{
    for (std::size_t i = 0; i < M; i++)
    {
        for (std::size_t j = 0; j < N; j++)
            if constexpr (std::is_integral<T>::value)
                os << static_cast<int>(A(i, j)) << ' ';
            else
                os << A(i, j) << ' ';
        os << '\n';
    }
    return os;
}

}
