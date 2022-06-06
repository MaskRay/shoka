#include "gtest/gtest.h"

#include "../nd_array.h"
#include "../nd_vector.h"

TEST(NDArray, Constructor) {
  {
    auto a = NDArray<int, 2>::create();
    ASSERT_EQ(a.size(), 2);
  }
  {
    auto a = NDArray<int, 2, 3>::create();
    ASSERT_EQ(a.size(), 2);
    ASSERT_EQ(a[0].size(), 3);
    ASSERT_EQ(a[1].size(), 3);
  }
}

TEST(NDVector, Constructor) {
  {
    auto v = NDVector<int, 3>::create(1, 2, 3);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0].size(), 2);
    ASSERT_EQ(v[0][0].size(), 3);
    ASSERT_EQ(v[0][1].size(), 3);
  }
  {
    auto v = NDVector<int, 2>::create(2, 3, 233);
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 3; ++j) {
        ASSERT_EQ(v[i][j], 233);
      }
    }
  }
}
