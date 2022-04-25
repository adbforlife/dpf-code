#include <vector>
#include "utils.h"
#include "dpf-tree.h"

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

TEST(DPFTest, Xor) {
  Seed s0{};
  Seed s1{};
  memset(&s0.s, 0, 16);
  memset(&s1.s, 0, 16);
  s0.s[0] = 10;
  s0.s[5] = 23;
  s1.s[1] = 40;
  s1.s[5] = 55;
  Seed s = XOR(s0, s1);
  EXPECT_EQ(s.s[0], 10);
  EXPECT_EQ(s.s[1], 40);
  EXPECT_EQ(s.s[2], 0);
  EXPECT_EQ(s.s[5], 23 ^ 55);

  uint8_t b0[10];
  uint8_t b1[5];
  memset(b0, 0, 10);
  memset(b1, 0, 5ll);
  b0[0] = 33;
  b0[1] = 35;
  b1[0] = 6;
  uint8_t* b = XOR(b0, b1, 5);
  EXPECT_EQ(b[0], 33 ^ 6);
  EXPECT_EQ(b[1], 35);

  std::vector<bool> t0 = {true, false, false};
  std::vector<bool> t1 = {true, true, false};
  auto t = XOR(t0, t1);
  EXPECT_EQ(t[0], 0);
  EXPECT_EQ(t[1], 1);
  EXPECT_EQ(t[2], 0);
}

TEST(DPFTest, Correctness) {
  std::srand(std::time(nullptr));
  for (int j = 0; j < 20; ++j) {
    int p = 5;
    int m = 10;
    DpfTree T = DpfTree(p, m);

    std::vector<bool> alpha;
    for (int i = 0; i < m; ++i) {
      alpha.push_back(std::rand() & 1);
    }

    std::vector<Key> ks = T.Gen(alpha);
    int p0 = std::rand() % p;
    int p1 = std::rand() % p;
    while (p1 == p0) p1 = std::rand() % p;
    Seed s0 = T.Eval(ks[p0], alpha);
    Seed s1 = T.Eval(ks[p1], alpha);
    EXPECT_EQ(T.Rec(s0, s1), 1);

    std::vector<bool> beta;
    for (int i = 0; i < m; ++i) {
      beta.push_back(std::rand() & 1);
    }
    std::cout << "alpha: ";
    print_bits(alpha);
    std::cout << "beta: ";
    print_bits(beta);
    bool is_equal = true;
    for (int i = 0; i < m; ++i) {
      if (alpha[i] != beta[i]) is_equal = false;
    }

    p0 = std::rand() % p;
    p1 = std::rand() % p;
    while (p1 == p0) p1 = std::rand() % p;
    std::cout << "p0: " << p0 << ", p1: " << p1 << "\n";
    s0 = T.Eval(ks[p0], beta);
    s1 = T.Eval(ks[p1], beta);
    EXPECT_EQ(T.Rec(s0, s1), is_equal);
  }
}
