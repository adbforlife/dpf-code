#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

// Generate random AES key
void gen_key(unsigned char* buf);
// PRG
unsigned char* G(unsigned char* key, uint32_t plen);
// Given number of parties, return byte length of PRG output
uint32_t blen(uint32_t p);
// Parse bytes into s's and t's. Assume bs, s0, s1 has correct length
void parse(uint8_t* bs, uint8_t* s0, uint8_t* s1, std::vector<bool>& ts, uint32_t p);
