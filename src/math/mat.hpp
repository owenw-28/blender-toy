#pragma once

#include "math/vec.hpp"

#include <array>
#include <cassert>

namespace bt {

template <typename T, int N>
struct Mat {
    static_assert(N == 3 || N == 4, "Mat only supports 3x3 and 4x4 matrices");
    using col_type = Vec<T, N>;

    std::array<col_type, N> cols{};

    constexpr Mat() = default;

    constexpr Mat(const col_type& c0, const col_type& c1, const col_type& c2) requires(N == 3) : cols{c0, c1, c2} {}
    constexpr Mat(const col_type& c0, const col_type& c1, const col_type& c2, const col_type& c3) requires(N == 4) : cols{c0, c1, c2, c3} {}

    [[nodiscard]] static constexpr Mat identity() {
        Mat result;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result.cols[i][j] = (i == j) ? T(1) : T(0);
            }
        }
        return result;
    }

    [[nodiscard]] constexpr col_type& operator[](int i) {
        assert(i >= 0 && i < N);
        return cols[i];
    }

    [[nodiscard]] constexpr const col_type& operator[](int i) const {
        assert(i >= 0 && i < N);
        return cols[i];
    }

    [[nodiscard]] const T* data() const { return &cols[0][0]; }

    constexpr Mat& operator*=(const Mat& other) {
        *this = *this * other;
        return *this;
    }
};

template <typename T, int N>
[[nodiscard]] constexpr Vec<T, N> operator*(const Mat<T, N>& m, const Vec<T, N>& v) {
    Vec<T, N> result;
    for (int i = 0; i < N; ++i) {
        result += m[i] * v[i];
    }
    return result;
}

template <typename T, int N>
[[nodiscard]] constexpr Mat<T, N> operator*(const Mat<T, N>& a, const Mat<T, N>& b) {
    Mat<T, N> result;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = T(0);
            for (int k = 0; k < N; ++k) {
                result[i][j] += a[k][j] * b[i][k];
            }
        }
    }
    return result;
}

template <typename T, int N>
[[nodiscard]] constexpr bool operator==(const Mat<T, N>& a, const Mat<T, N>& b) {
    for (int i = 0; i < N; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

template <typename T, int N>
[[nodiscard]] constexpr Mat<T, N> transpose(const Mat<T, N>& m) {
    Mat<T, N> result;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = m[j][i];
        }
    }
    return result;
}

template <typename T>
[[nodiscard]] constexpr Vec<T, 3> transform_point(const Mat<T, 4>& m, const Vec<T, 3>& p) {
    const Vec<T, 4> transformed = m * Vec<T, 4>{p.x, p.y, p.z, T(1)};
    return {transformed.x, transformed.y, transformed.z};
}

template <typename T>
[[nodiscard]] constexpr Vec<T, 3> transform_direction(const Mat<T, 4>& m, const Vec<T, 3>& v) {
    Vec<T, 4> v_homogeneous{v.x, v.y, v.z, T(0)};
    Vec<T, 4> transformed = m * v_homogeneous;
    return {transformed.x, transformed.y, transformed.z};
}

using float3x3 = Mat<float, 3>;
using float4x4 = Mat<float, 4>;

} // namespace bt