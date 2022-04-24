#include "utils.h"

#include "gtest/gtest.h"

TEST(DPFTest, PRG) {
  unsigned char* key = new unsigned char[16];
  key[3] = 3;
  key[5] = 5;
  unsigned char* c1;
  unsigned char* c2;
  unsigned char* c3;
  unsigned char* c4;
  c1 = G(key, 10);
  c2 = G(key, 16);
  c3 = G(key, 20);
  c4 = G(key, 40);
  EXPECT_EQ(c1[5], c2[5]);
  EXPECT_EQ(c1[5], c3[5]);
  EXPECT_EQ(c1[5], c4[5]);
  EXPECT_EQ(c2[10], 242);
  EXPECT_EQ(c3[10], 242);
  EXPECT_EQ(c4[10], 242);
  EXPECT_EQ(c3[20], 173);
  EXPECT_EQ(c4[20], 173);
  EXPECT_EQ(c4[30], 27);
  EXPECT_EQ(c2[15], 153);
  EXPECT_EQ(c3[15], 153);
  EXPECT_EQ(c4[15], 153);
}
