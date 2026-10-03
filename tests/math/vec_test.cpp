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

