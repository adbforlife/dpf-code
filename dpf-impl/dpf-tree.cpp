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
  std::vector<std::vector<bool>> ts;
  for (int j = 1; j < p_+1; ++j) {
    ts.push_back({});
    for (int k = 2; k < p_+1; ++k) {
      if (k == j) ts[j].push_back(1);
      else ts[j].push_back(0);
    }
  }

  // 3. Starting loop for each index bit
  for (int i = 1; i < m_+1; ++i) {
    // 4. PRG and parse
    std::vector<Seed> ss0;
    std::vector<Seed> ss1;
    std::vector<std::vector<bool>> ts;
    for (int j = 0; j < p_; ++j) {
      ss0.push_back({});
      ss1.push_back({});
      ts.push_back({});
      uint8_t* ciphertext = G(ss[j], blen(p_));
      parse(ciphertext, ss0[j], ss1[j], ts[j], p_); 
    }
    // 5. Correction words for seeds
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