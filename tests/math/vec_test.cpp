#include "math/vec.hpp"

#include <gtest/gtest.h>

using namespace bt;

static_assert(sizeof(float3) == 3 * sizeof(float));
static_assert(float3{1.0f, 2.0f, 3.0f} + float3{1.0f, 1.0f, 1.0f} == float3{2.0f, 3.0f, 4.0f});

TEST(math_vec, default_is_zero)
{
    const float3 v;
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
}

TEST(math_vec, fill_constructor)
{
    const float4 v(2.5f);
    for (int i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 2.5f);
    }
}

TEST(math_vec, index_matches_named_members)
{
    float3 v{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(v[0], v.x);
    EXPECT_EQ(v[1], v.y);
    EXPECT_EQ(v[2], v.z);

    v[1] = 5.0f;
    EXPECT_EQ(v.y, 5.0f);
}

TEST(math_vec, add)
{
    const float3 a{1.0f, 2.0f, 3.0f};
    const float3 b{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(a + b, (float3{5.0f, 7.0f, 9.0f}));
}

TEST(math_vec, add_int)
{
    EXPECT_EQ((int2{1, 2} + int2{3, 4}), (int2{4, 6}));
}

static_assert(float3{4.0f, 5.0f, 6.0f} - float3{1.0f, 2.0f, 3.0f} == float3{3.0f, 3.0f, 3.0f});
static_assert(dot(float3{1.0f, 2.0f, 3.0f}, float3{4.0f, 5.0f, 6.0f}) == 32.0f);
static_assert(cross(float3{1.0f, 0.0f, 0.0f}, float3{0.0f, 1.0f, 0.0f}) == float3{0.0f, 0.0f, 1.0f});

TEST(math_vec, subtract)
{
    const float3 a{4.0f, 5.0f, 6.0f};
    const float3 b{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(a - b, (float3{3.0f, 3.0f, 3.0f}));
}

TEST(math_vec, add_assign)
{
    float3 a{1.0f, 2.0f, 3.0f};
    a += float3{1.0f, 1.0f, 1.0f};
    EXPECT_EQ(a, (float3{2.0f, 3.0f, 4.0f}));
}

TEST(math_vec, add_assign_returns_self)
{
    float3 a{1.0f, 1.0f, 1.0f};
    float3& ref = (a += float3{1.0f, 1.0f, 1.0f});
    EXPECT_EQ(&ref, &a);
}

TEST(math_vec, scale_both_orders)
{
    const float3 v{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(v * 2.0f, (float3{2.0f, 4.0f, 6.0f}));
    EXPECT_EQ(2.0f * v, (float3{2.0f, 4.0f, 6.0f}));
    EXPECT_EQ(v * 2, (float3{2.0f, 4.0f, 6.0f})); // int scalar converts to float
}

TEST(math_vec, dot)
{
    EXPECT_EQ(dot(float3{1.0f, 2.0f, 3.0f}, float3{4.0f, 5.0f, 6.0f}), 32.0f);
    // Perpendicular vectors have a dot product of zero.
    EXPECT_EQ(dot(float3{1.0f, 0.0f, 0.0f}, float3{0.0f, 1.0f, 0.0f}), 0.0f);
}

TEST(math_vec, cross_is_right_handed)
{
    const float3 x{1.0f, 0.0f, 0.0f};
    const float3 y{0.0f, 1.0f, 0.0f};
    const float3 z{0.0f, 0.0f, 1.0f};
    EXPECT_EQ(cross(x, y), z);
    EXPECT_EQ(cross(y, z), x);
    EXPECT_EQ(cross(z, x), y);
    EXPECT_EQ(cross(y, x), z * -1.0f); // order matters
    EXPECT_EQ(cross(y, x), -z); // order matters
}

TEST(math_vec, cross_is_perpendicular)
{
    const float3 a{1.0f, 2.0f, 3.0f};
    const float3 b{-2.0f, 0.5f, 4.0f};
    const float3 c = cross(a, b);
    EXPECT_NEAR(dot(c, a), 0.0f, 1e-5f);
    EXPECT_NEAR(dot(c, b), 0.0f, 1e-5f);
}

TEST(math_vec, length)
{
    EXPECT_FLOAT_EQ(length(float3{3.0f, 4.0f, 0.0f}), 5.0f);
    EXPECT_EQ(length_squared(float3{3.0f, 4.0f, 0.0f}), 25.0f);
}

TEST(math_vec, normalize)
{
    const float3 n = normalize(float3{3.0f, 4.0f, 0.0f});
    EXPECT_NEAR(length(n), 1.0f, 1e-6f);
    EXPECT_NEAR(n.x, 0.6f, 1e-6f);
    EXPECT_NEAR(n.y, 0.8f, 1e-6f);
    EXPECT_NEAR(n.z, 0.0f, 1e-6f);
}

TEST(math_vec, normalize_zero_returns_zero)
{
    EXPECT_EQ(normalize(float3{}), float3{});
}

static_assert(-float3{1.0f, -2.0f, 3.0f} == float3{-1.0f, 2.0f, -3.0f});

TEST(math_vec, negate)
{
    const float3 v{1.0f, -2.0f, 3.0f};
    EXPECT_EQ(-v, (float3{-1.0f, 2.0f, -3.0f}));
    EXPECT_EQ(-(-v), v);              // negating twice gives the original
    EXPECT_EQ(v + (-v), float3{});    // a vector plus its opposite is zero
}

TEST(math_vec, subtract_assign)
{
    float3 a{5.0f, 5.0f, 5.0f};
    a -= float3{1.0f, 2.0f, 3.0f};
    EXPECT_EQ(a, (float3{4.0f, 3.0f, 2.0f}));
}

TEST(math_vec, scale_assign)
{
    float3 a{1.0f, 2.0f, 3.0f};
    a *= 2.0f;
    EXPECT_EQ(a, (float3{2.0f, 4.0f, 6.0f}));
}

TEST(math_vec, divide)
{
    EXPECT_EQ((float3{2.0f, 4.0f, 6.0f} / 2.0f), (float3{1.0f, 2.0f, 3.0f}));
    EXPECT_EQ((int2{7, 8} / 2), (int2{3, 4})); // integer division truncates
}

TEST(math_vec, normalize_small_is_accurate)
{
    // Small but still full precision: must normalize correctly.
    const float3 n = normalize(float3{1e-18f, 1e-18f, 0.0f});
    EXPECT_NEAR(n.x, 0.70710678f, 1e-6f);
    EXPECT_NEAR(n.y, 0.70710678f, 1e-6f);
}

TEST(math_vec, normalize_tiny_returns_zero)
{
    // Squared length (2e-40) is below the smallest normal float: too imprecise to normalize.
    EXPECT_EQ(normalize(float3{1e-20f, 1e-20f, 0.0f}), float3{});
}

TEST(math_vec2, construct_and_index)
{
    const float2 v{1.0f, 2.0f};
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 2.0f);
    EXPECT_EQ(v[0], v.x);
    EXPECT_EQ(v[1], v.y);
    EXPECT_EQ(float2::size(), 2);
}

TEST(math_vec2, arithmetic)
{
    const float2 a{1.0f, 2.0f};
    const float2 b{3.0f, 5.0f};
    EXPECT_EQ(a + b, (float2{4.0f, 7.0f}));
    EXPECT_EQ(b - a, (float2{2.0f, 3.0f}));
    EXPECT_EQ(a * 3.0f, (float2{3.0f, 6.0f}));
    EXPECT_EQ(-a, (float2{-1.0f, -2.0f}));
}

TEST(math_vec2, dot_length_normalize)
{
    const float2 v{3.0f, 4.0f};
    EXPECT_EQ(dot(v, float2{1.0f, 1.0f}), 7.0f);
    EXPECT_FLOAT_EQ(length(v), 5.0f);

    const float2 n = normalize(v);
    EXPECT_NEAR(n.x, 0.6f, 1e-6f);
    EXPECT_NEAR(n.y, 0.8f, 1e-6f);
}

TEST(math_vec4, construct_and_index)
{
    float4 v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_EQ(v[0], 1.0f);
    EXPECT_EQ(v[1], 2.0f);
    EXPECT_EQ(v[2], 3.0f);
    EXPECT_EQ(v[3], 4.0f);  // the only test that reaches the w branch of operator[]
    EXPECT_EQ(float4::size(), 4);

    v[3] = 9.0f;
    EXPECT_EQ(v.w, 9.0f);   // writing through [3] must change w, not another member
}

TEST(math_vec4, arithmetic)
{
    const float4 a{1.0f, 2.0f, 3.0f, 4.0f};
    const float4 b{4.0f, 3.0f, 2.0f, 1.0f};
    EXPECT_EQ(a + b, float4(5.0f));
    EXPECT_EQ(a - b, (float4{-3.0f, -1.0f, 1.0f, 3.0f}));
    EXPECT_EQ(2.0f * a, (float4{2.0f, 4.0f, 6.0f, 8.0f}));
    EXPECT_EQ(-a, (float4{-1.0f, -2.0f, -3.0f, -4.0f}));
}

TEST(math_vec4, dot_length_normalize)
{
    const float4 ones(1.0f);
    EXPECT_EQ(dot(ones, ones), 4.0f);
    EXPECT_FLOAT_EQ(length(ones), 2.0f);     // sqrt(1 + 1 + 1 + 1)

    const float4 n = normalize(ones);
    for (int i = 0; i < 4; ++i) {
        EXPECT_NEAR(n[i], 0.5f, 1e-6f);
    }
}
