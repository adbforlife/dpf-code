#include <vector>
#include "utils.h"

#include "gtest/gtest.h"

TEST(DPFTest, PRG) {
  Seed seed{};
  memset(&seed.s, 0, 16);
  seed.s[3] = 3;
  seed.s[5] = 5;
  unsigned char* c1;
  unsigned char* c2;
  unsigned char* c3;
  unsigned char* c4;
  c1 = G(seed, 10);
  c2 = G(seed, 16);
  c3 = G(seed, 20);
  c4 = G(seed, 40);
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

TEST(DPFTest, Parse) {
  Seed seed{};
  memset(&seed.s, 0, 16);
  seed.s[3] = 3;
  seed.s[5] = 5;

  uint32_t p = 6;
  uint32_t len = blen(p);
  EXPECT_EQ(len, 34);
  uint8_t* res = G(seed, len);
  Seed s0;
  Seed s1;
  std::vector<bool> ts;
  parse(res, s0, s1, ts, p);
  EXPECT_EQ(res[10], 242);
  EXPECT_EQ(res[20], 173);
  EXPECT_EQ(s0.s[10], 242);
  EXPECT_EQ(s1.s[4], 173);
  ASSERT_EQ(ts.size(), (p-1) * 2);
  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ(ts[i], (122 >> i) & 1);
  }
  for (int i = 0; i < 2; ++i) {
    EXPECT_EQ(ts[i+8], (29 >> i) & 1);
  }
}
