#include <iostream>
#include <iomanip>
#include <string.h>

#include "utils.h"

void handleErrors()
{
  // ERR_print_errors_fp(stderr);
  std::cerr << "OpenSSL failure\n";
  abort();
}

void gen_key(unsigned char* buf) {
  if (RAND_bytes(buf, 16) != 1)
    handleErrors();
}

unsigned char* G(const Seed& seed, uint32_t plen)
{
  EVP_CIPHER_CTX *ctx;
  int len;
  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new()))
      handleErrors();

  /*
    * Initialise the aes-ctr-128
    */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, seed.s, NULL))
      handleErrors();

  /*
   * Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  plen = (plen + 15) / 16 * 16;
  unsigned char* plaintext = new unsigned char[plen];
  memset(plaintext, 0, plen);
  unsigned char* ciphertext = new unsigned char[plen];

  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plen))
      handleErrors();

  /*
   * Finalise the encryption with padding (shouldn't exist).
   * Still called to clean up things in memory
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
      handleErrors();

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);
  return ciphertext;
}

uint32_t blen(uint32_t p) {
  return 16 * 2 + (2 * (p - 1) + 7) / 8;
}

void parse(uint8_t* bs, Seed& s0, Seed& s1, std::vector<bool>& ts, uint32_t p) {
  uint32_t len = blen(p); 
  memcpy(&s0.s, bs, 16);
  memcpy(&s1.s, bs+16, 16);
  for (int i = 32; i < len; ++i) {
    uint8_t b = bs[i];
    for (int j = 0; j < 8; ++j) {
      ts.push_back((b >> j) & 1);
      if (ts.size() == (p-1) * 2) return;
    }
  }
}

Seed XOR(const Seed& s0, const Seed& s1) {
  Seed res{};
  for (int i = 0; i < 16; ++i) {
    res.s[i] = s0.s[i] ^ s1.s[i];
  }
  return res;
}

uint8_t* XOR(const uint8_t* bs0, const uint8_t* bs1, uint32_t len) {
  uint8_t* res = new uint8_t[len];
  for (int i = 0; i < len; ++i) {
    res[i] = bs0[i] ^ bs1[i];
  }
  return res;
}

std::vector<bool> XOR(std::vector<bool> t0, std::vector<bool> t1) {
  std::vector<bool> res;
  for (int i = 0; i < t0.size(); ++i) {
    res.push_back(t0[i] ^ t1[i]);
  }
  return res;
}

void print_bits(const std::vector<bool>& t) {
  for (int i = 0; i < t.size(); ++i) {
    std::cout << t[i];
  }
  std::cout << "\n";
}

void print_seed(const Seed& s) {
  for (int i = 0; i < 16; ++i) {
    printf("%02x", s.s[i]);
  }
  std::cout << "\n";
}
