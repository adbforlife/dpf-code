#include <iostream>
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

unsigned char* G(unsigned char *key, uint32_t plen)
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
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, NULL))
      handleErrors();

  /*
    * Provide the message to be encrypted, and obtain the encrypted output.
    * EVP_EncryptUpdate can be called multiple times if necessary
    */
  plen = (plen + 15) / 16;
  unsigned char* plaintext = new unsigned char[plen];
  memset(plaintext, 0, plen);
  unsigned char* ciphertext = new unsigned char[plen];

  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plen))
      handleErrors();

  /*
    * Finalise the encryption with padding (shouldn't exist)
    */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
      handleErrors();

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext;
}