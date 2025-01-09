#pragma once

#include <random>
#include <array>
#include <mutex>
#include <memory>
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace crypto {

// Forward declarations
struct Hash;
void cn_fast_hash(const void *data, size_t length, Hash &hash);

class SecureRandom {
public:
    static SecureRandom& instance() {
        static SecureRandom instance;
        return instance;
    }

    template<typename T>
    T generate() {
        static_assert(std::is_unsigned<T>::value, "Only unsigned integer types are supported");
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        T result;
        if (!getRandomBytes(reinterpret_cast<uint8_t*>(&result), sizeof(T))) {
            throw std::runtime_error("Failed to generate random number");
        }
        return result;
    }

    void generateBytes(uint8_t* buffer, size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!getRandomBytes(buffer, size)) {
            throw std::runtime_error("Failed to generate random bytes");
        }
    }

    template<size_t N>
    std::array<uint8_t, N> generateBytes() {
        std::array<uint8_t, N> result;
        generateBytes(result.data(), N);
        return result;
    }

private:
    SecureRandom() {
        initializeEntropy();
    }

    ~SecureRandom() {
        // Cleanup entropy pool
        std::fill(entropy_pool_.begin(), entropy_pool_.end(), 0);
    }

    void initializeEntropy() {
        // Initialize entropy pool with system entropy
        std::random_device rd;
        for (size_t i = 0; i < entropy_pool_.size(); ++i) {
            entropy_pool_[i] = static_cast<uint8_t>(rd());
        }

        // Mix in additional entropy sources
        addSystemSpecificEntropy();
    }

    void addSystemSpecificEntropy() {
#ifdef _WIN32
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        mix(reinterpret_cast<uint8_t*>(&ft), sizeof(ft));

        LARGE_INTEGER perf;
        QueryPerformanceCounter(&perf);
        mix(reinterpret_cast<uint8_t*>(&perf), sizeof(perf));
#else
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        mix(reinterpret_cast<uint8_t*>(&ts), sizeof(ts));
#endif
    }

    void mix(const uint8_t* data, size_t size) {
        // Mix new entropy into the pool using SHA3
        Hash hash;
        cn_fast_hash(data, size, hash);
        
        size_t copySize = entropy_pool_.size();
        if (sizeof(Hash) < copySize) {
            copySize = sizeof(Hash);
        }
        
        for (size_t i = 0; i < copySize; ++i) {
            entropy_pool_[i] ^= reinterpret_cast<const uint8_t*>(&hash)[i];
        }
    }

    bool getRandomBytes(uint8_t* buffer, size_t size) {
        // Use the entropy pool to generate random bytes
        while (size > 0) {
            Hash hash;
            cn_fast_hash(entropy_pool_.data(), entropy_pool_.size(), hash);
            
            size_t copySize = size;
            if (sizeof(Hash) < copySize) {
                copySize = sizeof(Hash);
            }
            
            std::memcpy(buffer, &hash, copySize);
            buffer += copySize;
            size -= copySize;
            
            // Update entropy pool
            mix(reinterpret_cast<const uint8_t*>(&hash), sizeof(hash));
        }
        
        // Always update entropy after generation
        addSystemSpecificEntropy();
        return true;
    }

    std::mutex mutex_;
    std::array<uint8_t, 256> entropy_pool_;
};

} // namespace crypto
