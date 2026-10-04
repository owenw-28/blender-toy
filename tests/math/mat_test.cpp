#include "math/mat.hpp"

#include <gtest/gtest.h>

using namespace bt;

namespace {

// Compare every element with a tolerance (computed floats are rarely exact).
template <typename T, int N>
void expect_near(const Mat<T, N>& actual, const Mat<T, N>& expected, T tolerance = T(1e-5))
{
    for (int c = 0; c < N; ++c) {
        for (int r = 0; r < N; ++r) {
            EXPECT_NEAR(actual[c][r], expected[c][r], tolerance) << "at column " << c << ", row " << r;
        }
    }
}

// A non-symmetric 3x3, written by columns. In maths notation (rows) it is:
//   | 1 2  3 |
//   | 4 5  6 |
//   | 7 8 10 |
// Non-symmetric on purpose: a row/column mix-up gives a different answer.
constexpr float3x3 A{{1.0f, 4.0f, 7.0f}, {2.0f, 5.0f, 8.0f}, {3.0f, 6.0f, 10.0f}};

// | 2 0 1 |
// | 1 3 0 |
// | 0 1 4 |
constexpr float3x3 B{{2.0f, 1.0f, 0.0f}, {0.0f, 3.0f, 1.0f}, {1.0f, 0.0f, 4.0f}};

// A matrix that moves points by (5, -2, 3): identity with translation in column 3.
constexpr float4x4 translate_5_m2_3{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {5.0f, -2.0f, 3.0f, 1.0f},
};

} // namespace

static_assert(sizeof(float4x4) == 16 * sizeof(float), "float4x4 must be 16 packed floats for OpenGL");
static_assert(sizeof(float3x3) == 9 * sizeof(float));
static_assert(float4x4::identity()[2][2] == 1.0f);
static_assert(float4x4::identity()[2][1] == 0.0f);
static_assert(float3x3::identity() * float3{1.0f, 2.0f, 3.0f} == float3{1.0f, 2.0f, 3.0f});

TEST(math_mat, default_is_zero)
{
    const float4x4 m;
    for (int c = 0; c < 4; ++c) {
        EXPECT_EQ(m[c], float4{});
    }
}

TEST(math_mat, identity)
{
    const float4x4 m = float4x4::identity();
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            EXPECT_EQ(m[c][r], c == r ? 1.0f : 0.0f) << "at column " << c << ", row " << r;
        }
    }
}

TEST(math_mat, constructor_takes_columns)
{
    // m[col][row]: the second constructor argument is column 1.
    EXPECT_EQ(A[1], (float3{2.0f, 5.0f, 8.0f}));
    EXPECT_EQ(A[1][0], 2.0f); // column 1, row 0: top of the middle column
    EXPECT_EQ(A[0][1], 4.0f); // column 0, row 1: NOT the same element
}

TEST(math_mat, translation_lives_in_column_3)
{
    EXPECT_EQ(translate_5_m2_3[3], (float4{5.0f, -2.0f, 3.0f, 1.0f}));
}

TEST(math_mat, index_write)
{
    float4x4 m = float4x4::identity();
    m[3][0] = 7.0f;
    EXPECT_EQ(m[3].x, 7.0f);
    EXPECT_EQ(m[0][3], 0.0f); // the transposed element is untouched
}

TEST(math_mat, data_is_column_major)
{
    // OpenGL reads 16 floats in a row: column 0 first, then column 1, ...
    const float* d = translate_5_m2_3.data();
    EXPECT_EQ(d[0], 1.0f);  // column 0, row 0
    EXPECT_EQ(d[12], 5.0f); // column 3, row 0: translation x
    EXPECT_EQ(d[13], -2.0f);
    EXPECT_EQ(d[14], 3.0f);
    EXPECT_EQ(d[15], 1.0f);
}

// --- Matrix x vector -----------------------------------------------------------

TEST(math_mat, identity_times_vector)
{
    const float4 v{1.0f, -2.0f, 3.5f, 1.0f};
    EXPECT_EQ(float4x4::identity() * v, v);
}

TEST(math_mat, times_vector_by_hand)
{
    // Row 0: 1*1 + 2*2 + 3*3  = 14
    // Row 1: 4*1 + 5*2 + 6*3  = 32
    // Row 2: 7*1 + 8*2 + 10*3 = 53
    EXPECT_EQ((A * float3{1.0f, 2.0f, 3.0f}), (float3{14.0f, 32.0f, 53.0f}));
}

TEST(math_mat, times_basis_vector_picks_column)
{
    // M * (0, 1, 0) is exactly column 1.
    EXPECT_EQ((A * float3{0.0f, 1.0f, 0.0f}), A[1]);
}

// --- Matrix x matrix -----------------------------------------------------------

TEST(math_mat, identity_times_matrix)
{
    EXPECT_EQ(float3x3::identity() * A, A);
    EXPECT_EQ(A * float3x3::identity(), A);
}

TEST(math_mat, times_matrix_by_hand)
{
    // A * B in maths notation:
    //   |  4  9 13 |
    //   | 13 21 28 |
    //   | 22 34 47 |
    const float3x3 expected{{4.0f, 13.0f, 22.0f}, {9.0f, 21.0f, 34.0f}, {13.0f, 28.0f, 47.0f}};
    EXPECT_EQ(A * B, expected);
}

TEST(math_mat, multiplication_order_matters)
{
    EXPECT_NE(A * B, B * A);
}

TEST(math_mat, multiplication_is_associative)
{
    // (A * B) * C == A * (B * C): what lets a chain of transforms collapse into one matrix.
    const float3x3 C{{0.5f, -1.0f, 2.0f}, {1.5f, 0.25f, 0.0f}, {-2.0f, 1.0f, 3.0f}};
    expect_near((A * B) * C, A * (B * C));
}

TEST(math_mat, multiply_assign)
{
    float3x3 m = A;
    m *= B;
    EXPECT_EQ(m, A * B); // *= means *this = *this * other, so B applies first
}

TEST(math_mat, transpose_swaps_rows_and_columns)
{
    const float3x3 t = transpose(A);
    for (int c = 0; c < 3; ++c) {
        for (int r = 0; r < 3; ++r) {
            EXPECT_EQ(t[c][r], A[r][c]);
        }
    }
}

TEST(math_mat, transpose_twice_is_original)
{
    EXPECT_EQ(transpose(transpose(A)), A);
}

TEST(math_mat, transpose_of_identity_is_identity)
{
    EXPECT_EQ(transpose(float4x4::identity()), float4x4::identity());
}

TEST(math_mat, transform_point_applies_translation)
{
    EXPECT_EQ((transform_point(translate_5_m2_3, float3{1.0f, 1.0f, 1.0f})), (float3{6.0f, -1.0f, 4.0f}));
}

TEST(math_mat, transform_direction_ignores_translation)
{
    EXPECT_EQ((transform_direction(translate_5_m2_3, float3{1.0f, 1.0f, 1.0f})), (float3{1.0f, 1.0f, 1.0f}));
}

TEST(math_mat, transform_point_does_not_divide_by_w)
{
    // Bottom row (0, 0, 0, 2) makes the result's w = 2. transform_point is for
    // move/rotate/scale and must NOT do a perspective divide (that's project_point, M4).
    float4x4 m = float4x4::identity();
    m[3][3] = 2.0f;
    EXPECT_EQ((transform_point(m, float3{2.0f, 4.0f, 6.0f})), (float3{2.0f, 4.0f, 6.0f}));
}
