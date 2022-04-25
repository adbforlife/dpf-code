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

std::vector<Key> DpfTree::Gen(bits ind) {
  // 0: p keys
  std::vector<Key> keys;
  for (int i = 0; i < p_; ++i) {
    keys.push_back({});
  }

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
      if (k == j) ts[j-1].push_back(1);
      else ts[j-1].push_back(0);
    }
  }

  // 3. Add to keys
  for (int j = 0; j < p_; ++j) {
    keys[j].s0 = ss[j];
    keys[j].t0 = ts[j];
  }

  // 4. Starting loop for each index bit
  for (int i = 1; i < m_+1; ++i) {
    // 5. Determine alpha_i
    bool alpha = ind[i-1];
    // 6. PRG and parse
    std::vector<Seed> ss0;
    std::vector<Seed> ss1;
    std::vector<std::vector<bool>> tsi;
    for (int j = 0; j < p_; ++j) {
      ss0.push_back({});
      ss1.push_back({});
      tsi.push_back({});
      uint8_t* ciphertext = G(ss[j], blen(p_));
      parse(ciphertext, ss0[j], ss1[j], tsi[j], p_); 
    }
    // 7. Correction words for seeds
    std::vector<Seed> CWs;
    for (int k = 2; k < p_+1; ++k) {
      if (alpha) {
        CWs.push_back(XOR(ss0[0], ss0[k-1]));
      } else {
        CWs.push_back(XOR(ss1[0], ss1[k-1]));
      }
    }
    // 8. Correction words for control bits.
    std::vector<std::vector<bool>> CWt0;
    std::vector<std::vector<bool>> CWt1;
    for (int j = 2; j < p_+1; ++j) {
      CWt0.push_back({});
      CWt1.push_back({});
      for (int k = 2; k < p_+1; ++k) {
        if (j == k) {
          CWt0[j-2].push_back(tsi[0][k-2] ^ tsi[j-1][k-2] ^ alpha ^ 1);
          CWt1[j-2].push_back(tsi[0][k-2+p_-1] ^ tsi[j-1][k-2+p_-1] ^ alpha);
        } else {
          CWt0[j-2].push_back(tsi[0][k-2] ^ tsi[j-1][k-2]);
          CWt1[j-2].push_back(tsi[0][k-2+p_-1] ^ tsi[j-1][k-2+p_-1]);
        }
      }
    }
    // 9. CW
    std::vector<CWBase> CW;
    for (int j = 2; j < p_+1; ++j) {
      CW.push_back({});
      CW[j-2].s = CWs[j-2];
      CW[j-2].ts0 = CWt0[j-2];
      CW[j-2].ts1 = CWt1[j-2];
    }
    // 10. new s/t
    auto old_ts = ts;
    for (int j = 0; j < p_; ++j) {
      if (alpha) {
        ss[j] = ss1[j];
        for (int l = 2; l < p_+1; ++l) {
          ts[j][l-2] = tsi[j][l-2+p_-1];
        }
      } else {
        ss[j] = ss0[j];
        for (int l = 2; l < p_+1; ++l) {
          ts[j][l-2] = tsi[j][l-2];
        }
      }

      for (int k = 2; k < p_+1; ++k) {
        if (old_ts[j][k-2]) {
          ss[j] = XOR(ss[j], CW[k-2].s);
          for (int l = 2; l < p_+1; ++l) {
            if (alpha) {
              ts[j][l-2] = ts[j][l-2] ^ CW[k-2].ts1[l-2];
            } else {
              ts[j][l-2] = ts[j][l-2] ^ CW[k-2].ts0[l-2];
            }
          }
        }
      }
    }
    // 11. Add to CW
    for (int j = 0; j < p_; ++j)
      keys[j].CWm.push_back(CW);
  }
  // 12. CW_{m+1}
  for (int j = 2; j < p_+1; ++j) {
    // Let's assume p < 256 here.
    for (int l = 0; l < p_; ++l) {
      keys[l].sm.push_back(XOR(ss[0], ss[j-1]));
      keys[l].sm[j-2].s[15] ^= j;
    }
  }
  return keys;
}

Seed DpfTree::Eval(Key k, bits ind) {
  Seed s = k.s0;
  std::vector<bool> t = k.t0;
  for (int i = 0; i < m_; ++i) {
    bool beta = ind[i];
    // 1. PRG
    Seed s0, s1;
    std::vector<bool> ti;
    uint8_t* ciphertext = G(s, blen(p_));
    parse(ciphertext, s0, s1, ti, p_); 
    if (beta) s = s1;
    else s = s0;
    // 2. tau
    auto old_t = t;
    for (int j = 2; j < p_+1; ++j) {
      if (beta) {
        t[j-2] = ti[j-2+p_-1];
      } else {
        t[j-2] = ti[j-2];
      }
    }
    for (int j = 2; j < p_+1; ++j) {
      if (old_t[j-2]) {
        s = XOR(s, k.CWm[i][j-2].s);
        if (beta) {
          t = XOR(t, k.CWm[i][j-2].ts1);
        } else {
          t = XOR(t, k.CWm[i][j-2].ts0);
        }
      }
    }
  }
  // 3. CW_{m+1}
  for (int j = 2; j < p_+1; ++j) {
    if (t[j-2]) {
      s = XOR(s, k.sm[j-2]);
    }
  }
  return s;
}

bool DpfTree::Rec(Seed s0, Seed s1) {
  Seed s = XOR(s0, s1);
  for (int i = 0; i < 16; ++i) {
    if (s.s[i]) return true;
  }
  return false;
}

/*
int main() {
  DpfTree t = DpfTree(5, 10);
  bits ind = {0, 1, 0, 0, 1, 1, 0, 0, 1, 1};
  t.Gen(ind);

  // G(NULL, 3, NULL, NULL, NULL);
  std::cout << "hi\n";
  return 0;
}
*/
