#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

void gen_key(unsigned char* buf);

unsigned char* G(unsigned char *key, uint32_t plen);