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
  // 1: Generate seeds
  std::vector<Seed> ss;
  for (int i = 0; i < p_; ++i) {
    ss.push_back({});
    gen_key(ss[i].s);
  }

  // 2: Initialize control bits
  std::vector<std::vector<uint8_t>> ts;
  for (int j = 1; j < p_+1; ++j) {
    ts.push_back({});
    for (int k = 2; k < p_+1; ++k) {
      if (k == j) ts[j].push_back(1);
      else ts[j].push_back(0);
    }
  }

  // 3. Starting loop for each index bit
  for (int i = 1; i < m_+1; ++i) {
    continue;
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