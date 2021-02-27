#include "gtest/gtest.h"

#include "vector.hh"

TEST(vector, sum) {
    auto v1 = isim::Vector3(1, 2, 3);
    auto v2 = isim::Vector3(1, 2, 3);
    auto v3 = v1 + v2;

    ASSERT_EQ(v3.get_x(), 2);
    ASSERT_EQ(v3.get_y(), 4);
    ASSERT_EQ(v3.get_z(), 6);
}