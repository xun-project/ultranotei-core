#include "crypto.h"
#include <openssl/sha.h>
#include <openssl/aes.h>

// SHA-256 implementation using OpenSSL
int crypto_hash_sha256(unsigned char *out, const unsigned char *in, unsigned long long inlen) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, in, inlen);
    SHA256_Final(out, &sha256);
    return 0;
}

// AES encryption implementation using OpenSSL
void aes_encrypt(const uint8_t* key, const uint8_t* in, uint8_t* out) {
    AES_KEY aesKey;
    AES_set_encrypt_key(key, 256, &aesKey);
    AES_encrypt(in, out, &aesKey);
}
