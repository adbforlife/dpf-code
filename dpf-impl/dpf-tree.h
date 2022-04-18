#include <vector>

typedef std::vector<bool> bits;

struct Seed {
  unsigned char s[16];
};

class DpfTree {
  public:
    // p: number of parties, m: 2^m items, lambda: sec param
    DpfTree(int p, int m, int lambda = 128);
    bits Gen(bits ind);
    bits Eval(bits k, bits ind);
    bits Rec(std::vector<bits> shares);
  private:
    int p_;
    int m_;
};