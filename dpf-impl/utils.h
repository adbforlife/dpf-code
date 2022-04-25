#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

struct Seed {
  uint8_t s[16];
};

// Generate random AES key
void gen_key(unsigned char* buf);
// PRG
unsigned char* G(const Seed& seed, uint32_t plen);
// Given number of parties, return byte length of PRG output
uint32_t blen(uint32_t p);
// Parse bytes into s's and t's. Assume bs has correct length and ts empty
void parse(uint8_t* bs, Seed& s0, Seed& s1, std::vector<bool>& ts, uint32_t p);
// Xoring bytes
uint8_t* XOR(const uint8_t* bs0, const uint8_t* bs1, uint32_t len);
// Xoring seeds
Seed XOR(const Seed& s0, const Seed& s1);
// Xoring bits
std::vector<bool> XOR(std::vector<bool> t0, std::vector<bool> t1);


/* Debug Use */
void print_bits(const std::vector<bool>& t);
void print_seed(const Seed& s);

#endif // UTILS_H
