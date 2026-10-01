#pragma once

#include <cassert>

namespace bt {

namespace detail {

template <typename T, int N> struct VecStorage;

template <typename T> struct VecStorage<T, 2> { T x{}, y{}; };
template <typename T> struct VecStorage<T, 3> { T x{}, y{}, z{}; };
template <typename T> struct VecStorage<T, 4> { T x{}, y{}, z{}, w{}; };

} // namespace detail

template <typename T, int N>
struct Vec : detail::VecStorage<T, N> {
    static_assert(N >= 2 && N <= 4, "Vec only supports 2D, 3D, and 4D vectors");

    constexpr Vec() = default;

    constexpr explicit Vec(T value) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] = value;
        }
    }

    constexpr Vec(T x_, T y_) requires(N == 2) : detail::VecStorage<T, N>{x_, y_} {}
    constexpr Vec(T x_, T y_, T z_) requires(N == 3) : detail::VecStorage<T, N>{x_, y_, z_} {}
    constexpr Vec(T x_, T y_, T z_, T w_) requires(N == 4) : detail::VecStorage<T, N>{x_, y_, z_, w_} {}

    [[nodiscard]] constexpr T& operator[](int i) {
        assert(i >= 0 && i < N);
        if constexpr (N >= 4) {
            if (i == 3) return this->w;
        }
        if constexpr (N >= 3) {
            if (i == 2) return this->z;
        }
        if (i == 1) return this->y;
        return this->x;
    }

    [[nodiscard]] constexpr const T& operator[](int i) const {
        assert(i >= 0 && i < N);
        if constexpr (N >= 4) {
            if (i == 3) return this->w;
        }
        if constexpr (N >= 3) {
            if (i == 2) return this->z;
        }
        if (i == 1) return this->y;
        return this->x;
    }

    [[nodiscard]] static constexpr int size() { return N; }
};

template <typename T, int N>
[[nodiscard]] constexpr Vec<T, N> operator+(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> result;
    for (int i = 0; i < N; ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <typename T, int N>
[[nodiscard]] constexpr bool operator==(const Vec<T, N>& a, const Vec<T, N>& b) {
    for (int i = 0; i < N; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

using float2 = Vec<float, 2>;
using float3 = Vec<float, 3>;
using float4 = Vec<float, 4>;
using int2 = Vec<int, 2>;
using int3 = Vec<int, 3>;
using int4 = Vec<int, 4>;

} // namespace bt