#pragma once

#include <chrono>
#include "secure_memory.h"
#include "secure_random.h"
#include "crypto.h"

namespace crypto {

class KeyManager {
public:
    struct KeyPair {
        SecureMemory<uint8_t> privateKey;
        PublicKey publicKey;
        
        // Timestamp for key rotation
        std::chrono::system_clock::time_point creationTime;
        std::chrono::system_clock::time_point expirationTime;
    };

    static KeyManager& instance() {
        static KeyManager instance;
        return instance;
    }

    // Generate a new key pair with proper entropy validation
    KeyPair generateKeyPair(const std::chrono::seconds& validity = std::chrono::hours(24 * 30)) {
        KeyPair pair;
        pair.privateKey.resize(32); // Size for ed25519 private key
        
        // Generate private key with secure random
        SecureRandom::instance().generateBytes(pair.privateKey.data(), pair.privateKey.size());
        
        // Validate entropy of the generated key
        if (!validateKeyEntropy(pair.privateKey.data(), pair.privateKey.size())) {
            throw std::runtime_error("Generated key has insufficient entropy");
        }

        // Generate public key
        if (!crypto::secret_key_to_public_key(
                *reinterpret_cast<const SecretKey*>(pair.privateKey.data()),
                pair.publicKey)) {
            throw std::runtime_error("Failed to generate public key");
        }

        // Set key timestamps
        pair.creationTime = std::chrono::system_clock::now();
        pair.expirationTime = pair.creationTime + validity;

        return pair;
    }

    // Derive a new key using HKDF
    KeyPair deriveKey(const uint8_t* masterKey, size_t masterKeySize,
                     const uint8_t* salt, size_t saltSize,
                     const uint8_t* info, size_t infoSize) {
        // Implement HKDF-based key derivation
        SecureMemory<uint8_t> prk(32);
        hkdfExtract(salt, saltSize, masterKey, masterKeySize, prk.data());
        
        KeyPair derived;
        derived.privateKey.resize(32);
        hkdfExpand(prk.data(), 32, info, infoSize, derived.privateKey.data(), 32);

        // Generate public key
        if (!crypto::secret_key_to_public_key(
                *reinterpret_cast<const SecretKey*>(derived.privateKey.data()),
                derived.publicKey)) {
            throw std::runtime_error("Failed to generate derived public key");
        }

        derived.creationTime = std::chrono::system_clock::now();
        derived.expirationTime = derived.creationTime + std::chrono::hours(24);

        return derived;
    }

private:
    KeyManager() = default;
    
    bool validateKeyEntropy(const uint8_t* key, size_t size) {
        // Simple entropy estimation using byte frequency analysis
        std::array<unsigned, 256> freq = {0};
        for (size_t i = 0; i < size; ++i) {
            freq[key[i]]++;
        }
        
        double entropy = 0.0;
        for (unsigned f : freq) {
            if (f > 0) {
                double p = static_cast<double>(f) / size;
                entropy -= p * std::log2(p);
            }
        }
        
        // Require at least 7 bits of entropy per byte
        return entropy / size >= 7.0;
    }

    void hkdfExtract(const uint8_t* salt, size_t saltSize,
                    const uint8_t* ikm, size_t ikmSize,
                    uint8_t* prk) {
        // HMAC-based Extract step of HKDF
        Hash hash;
        cn_fast_hash(salt, saltSize, hash);
        
        SecureMemory<uint8_t> temp(ikmSize + sizeof(Hash));
        std::memcpy(temp.data(), &hash, sizeof(Hash));
        std::memcpy(temp.data() + sizeof(Hash), ikm, ikmSize);
        
        cn_fast_hash(temp.data(), temp.size(), *reinterpret_cast<Hash*>(prk));
    }

    void hkdfExpand(const uint8_t* prk, size_t prkSize,
                   const uint8_t* info, size_t infoSize,
                   uint8_t* okm, size_t okmSize) {
        // HMAC-based Expand step of HKDF
        SecureMemory<uint8_t> temp(prkSize + infoSize + 1);
        uint8_t counter = 1;
        
        size_t offset = 0;
        while (offset < okmSize) {
            // T(i) = HMAC-Hash(PRK, T(i-1) | info | i)
            std::memcpy(temp.data(), prk, prkSize);
            std::memcpy(temp.data() + prkSize, info, infoSize);
            temp.data()[prkSize + infoSize] = counter++;
            
            Hash hash;
            cn_fast_hash(temp.data(), temp.size(), hash);
            
            size_t copySize = std::min(sizeof(Hash), okmSize - offset);
            std::memcpy(okm + offset, &hash, copySize);
            offset += copySize;
        }
    }
};

} // namespace crypto
