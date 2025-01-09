#pragma once

#include <cstdint>
#include <vector>

namespace crypto {

struct Hash {
  uint8_t data[32];
};

struct PublicKey {
  uint8_t data[32];
};

struct SecretKey {
  uint8_t data[32];
};

struct KeyDerivation {
  uint8_t data[32];
};

struct KeyImage {
  uint8_t data[32];
};

struct Signature {
  uint8_t data[64];
};

} // namespace crypto

namespace cn {
  using crypto::Hash;
  using crypto::PublicKey;
  using crypto::SecretKey;
  using crypto::KeyDerivation;
  using crypto::KeyImage;
  using crypto::Signature;
} // namespace cn
