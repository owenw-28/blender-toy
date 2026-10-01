#include "math/vec.hpp"

#include <gtest/gtest.h>

using namespace bt;

static_assert(sizeof(float3) == 3 * sizeof(float));
static_assert(float3{1.0f, 2.0f, 3.0f} + float3{1.0f, 1.0f, 1.0f} == float3{2.0f, 3.0f, 4.0f});

TEST(math_vec, default_is_zero) {
    float3 v;
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