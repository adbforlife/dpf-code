#ifndef DPF_TREE_H
#define DPF_TREE_H

#include <vector>

typedef std::vector<bool> bits;

struct CWBase {
  Seed s;
  std::vector<bool> ts0;
  std::vector<bool> ts1;
};

struct Key {
  Seed s0;
  std::vector<bool> t0;
  std::vector<std::vector<CWBase>> CWm;
  std::vector<Seed> sm;
};

class DpfTree {
  public:
    // p: number of parties, m: 2^m items, lambda: sec param
    DpfTree(int p, int m, int lambda = 128);
    std::vector<Key> Gen(bits ind);
    Seed Eval(Key k, bits ind);
    // For DPF only, need update for PIR.
    bool Rec(Seed s0, Seed s1);
  private:
    int p_;
    int m_;
};

#endif // DPF_TREE_H
