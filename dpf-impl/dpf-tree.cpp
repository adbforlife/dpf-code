#include <iostream>
#include <cassert>

#include "utils.h"
#include "dpf-tree.h"

DpfTree::DpfTree(int p, int m, int lambda) : p_(p), m_(m) {
  if (lambda != 128) {
    std::cerr << "Key length not supported.\n";
    assert(0);
  }
}

bits DpfTree::Gen(bits ind) {
  // Line 2: Generate seeds
  std::vector<Seed> ss;
  for (int i = 0; i < p_; ++i) {
    ss.push_back({});
    gen_key(ss[i].s);
  }
  return {0,1};
}

int main() {
  DpfTree t = DpfTree(5, 10);
  bits ind = {0, 1, 0, 0, 1, 1, 0, 0, 1, 1};
  t.Gen(ind);
  // G(NULL, 3, NULL, NULL, NULL);
  std::cout << "hi\n";
  return 0;
}