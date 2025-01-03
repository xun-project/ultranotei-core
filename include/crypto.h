#ifndef CRYPTO_H
#define CRYPTO_H

#include <cstddef>
#include <cstdint>

// SHA-256 hash function
extern "C" {
    int crypto_hash_sha256(unsigned char *out, const unsigned char *in, unsigned long long inlen);
}

// AES encryption function
extern "C" {
    void aes_encrypt(const uint8_t* key, const uint8_t* in, uint8_t* out);
}

#endif // CRYPTO_H
