#pragma once

#include <cstddef>
#include <limits>
#include <mutex>
#include <type_traits>
#include <vector>

#include "CryptoTypes.h"

namespace crypto {

  extern "C" {
#include "hash-ops.h"
  }

  static_assert(sizeof(Hash) == HASH_SIZE, "Invalid structure size");
  static_assert(sizeof(PublicKey) == sizeof(EllipticCurvePoint), "Invalid structure size");
  static_assert(sizeof(SecretKey) == sizeof(EllipticCurveScalar), "Invalid structure size");
  static_assert(sizeof(KeyDerivation) == sizeof(EllipticCurvePoint), "Invalid structure size");
  static_assert(sizeof(KeyImage) == sizeof(EllipticCurvePoint), "Invalid structure size");
  static_assert(sizeof(Signature) == sizeof(EllipticCurveScalar) + sizeof(EllipticCurvePoint), "Invalid structure size");

  // Comparison operators for crypto types
  inline bool operator==(const SecretKey& _v1, const SecretKey& _v2) {
    return std::memcmp(&_v1, &_v2, sizeof(SecretKey)) == 0;
  }

  inline bool operator!=(const SecretKey& _v1, const SecretKey& _v2) {
    return !(_v1 == _v2);
  }

  inline bool operator==(const PublicKey& _v1, const PublicKey& _v2) {
    return std::memcmp(&_v1, &_v2, sizeof(PublicKey)) == 0;
  }

  inline bool operator!=(const PublicKey& _v1, const PublicKey& _v2) {
    return !(_v1 == _v2);
  }

  inline bool operator==(const KeyImage& _v1, const KeyImage& _v2) {
    return std::memcmp(&_v1, &_v2, sizeof(KeyImage)) == 0;
  }

  inline bool operator!=(const KeyImage& _v1, const KeyImage& _v2) {
    return !(_v1 == _v2);
  }

  // Hash functions for crypto types (for use in std::unordered_map/set)
  struct SecretKeyHash {
    size_t operator()(const SecretKey& key) const {
      return *reinterpret_cast<const size_t*>(&key);
    }
  };

  struct PublicKeyHash {
    size_t operator()(const PublicKey& key) const {
      return *reinterpret_cast<const size_t*>(&key);
    }
  };

  struct KeyImageHash {
    size_t operator()(const KeyImage& key) const {
      return *reinterpret_cast<const size_t*>(&key);
    }
  };

  struct Hash {
    uint8_t data[32];
  };

  struct PublicKey {
    uint8_t data[32];
  };

  struct SecretKey {
    uint8_t data[32];
  };

  struct KeyImage {
    uint8_t data[32];
  };

  struct Signature {
    uint8_t data[64];
  };

  struct SecureSignature {
    uint8_t data[64];
  };

  struct SecureKeyDerivation {
    uint8_t data[32];
  };

  class crypto_ops {
  public:
    static void generate_keys(PublicKey& pub, SecretKey& sec);
    static bool check_key(const PublicKey& key);
    static bool secret_key_to_public_key(const SecretKey& sec, PublicKey& pub);
    static bool generate_key_derivation(const PublicKey& key1, const SecretKey& key2, SecureKeyDerivation& derivation);
    static bool derive_public_key(const SecureKeyDerivation& derivation, size_t output_index,
                                const PublicKey& base, PublicKey& derived_key);
    static void derive_secret_key(const SecureKeyDerivation& derivation, size_t output_index,
                                const SecretKey& base, SecretKey& derived_key);
    static bool underive_public_key_and_get_scalar(const SecureKeyDerivation& derivation, size_t output_index,
                                                 const PublicKey& derived_key, PublicKey& base, EllipticCurveScalar& hashed_derivation);
    static void generate_signature(const Hash& prefix_hash, const PublicKey& pub, const SecretKey& sec, Signature& sig);
    static bool check_signature(const Hash& prefix_hash, const PublicKey& pub, const Signature& sig);
    static void generate_key_image(const PublicKey& pub, const SecretKey& sec, KeyImage& image);
    static void generate_ring_signature(const Hash& prefix_hash, const KeyImage& image,
                                      const PublicKey* const* pubs, size_t pubs_count,
                                      const SecretKey& sec, size_t sec_index,
                                      Signature* sig);
    static bool check_ring_signature(const Hash& prefix_hash, const KeyImage& image,
                                   const PublicKey* const* pubs, size_t pubs_count,
                                   const Signature* sig);
    static bool check_tx_proof(const Hash& prefix_hash, const PublicKey& R, const PublicKey& A, const SecureSignature& sig);
    static bool EllipticCurveScalar2SecretKey(const EllipticCurveScalar& scalar, SecretKey& secret_key);
  };

  inline void generate_keys(PublicKey& pub, SecretKey& sec) {
    crypto_ops::generate_keys(pub, sec);
  }

  inline bool check_key(const PublicKey& key) {
    return crypto_ops::check_key(key);
  }

  inline bool secret_key_to_public_key(const SecretKey& sec, PublicKey& pub) {
    return crypto_ops::secret_key_to_public_key(sec, pub);
  }

  inline bool generate_key_derivation(const PublicKey& key1, const SecretKey& key2, SecureKeyDerivation& derivation) {
    return crypto_ops::generate_key_derivation(key1, key2, derivation);
  }

  inline bool derive_public_key(const SecureKeyDerivation& derivation, size_t output_index,
                              const PublicKey& base, PublicKey& derived_key) {
    return crypto_ops::derive_public_key(derivation, output_index, base, derived_key);
  }

  inline void derive_secret_key(const SecureKeyDerivation& derivation, size_t output_index,
                              const SecretKey& base, SecretKey& derived_key) {
    crypto_ops::derive_secret_key(derivation, output_index, base, derived_key);
  }

  inline bool underive_public_key_and_get_scalar(const SecureKeyDerivation& derivation, size_t output_index,
                                               const PublicKey& derived_key, PublicKey& base, EllipticCurveScalar& hashed_derivation) {
    return crypto_ops::underive_public_key_and_get_scalar(derivation, output_index, derived_key, base, hashed_derivation);
  }

  inline void generate_signature(const Hash& prefix_hash, const PublicKey& pub, const SecretKey& sec, Signature& sig) {
    crypto_ops::generate_signature(prefix_hash, pub, sec, sig);
  }

  inline bool check_signature(const Hash& prefix_hash, const PublicKey& pub, const Signature& sig) {
    return crypto_ops::check_signature(prefix_hash, pub, sig);
  }

  inline void generate_key_image(const PublicKey& pub, const SecretKey& sec, KeyImage& image) {
    crypto_ops::generate_key_image(pub, sec, image);
  }

  inline void generate_ring_signature(const Hash& prefix_hash, const KeyImage& image,
                                    const PublicKey* const* pubs, size_t pubs_count,
                                    const SecretKey& sec, size_t sec_index,
                                    Signature* sig) {
    crypto_ops::generate_ring_signature(prefix_hash, image, pubs, pubs_count, sec, sec_index, sig);
  }

  inline bool check_ring_signature(const Hash& prefix_hash, const KeyImage& image,
                                 const PublicKey* const* pubs, size_t pubs_count,
                                 const Signature* sig) {
    return crypto_ops::check_ring_signature(prefix_hash, image, pubs, pubs_count, sig);
  }

  inline bool check_tx_proof(const Hash& prefix_hash, const PublicKey& R, const PublicKey& A, const SecureSignature& sig) {
    return crypto_ops::check_tx_proof(prefix_hash, R, A, sig);
  }

  inline bool EllipticCurveScalar2SecretKey(const EllipticCurveScalar& scalar, SecretKey& secret_key) {
    return crypto_ops::EllipticCurveScalar2SecretKey(scalar, secret_key);
  }

  inline void hash_to_scalar(const void* data, size_t length, EllipticCurveScalar& res) {
    cn_fast_hash(data, length, reinterpret_cast<Hash&>(res));
  }

  inline void scalarmultKey(PublicKey& output, const PublicKey& input, const SecretKey& secret) {
    ge_p3 point;
    ge_frombytes_vartime(&point, reinterpret_cast<const unsigned char*>(&input));
    ge_scalarmult(&point, reinterpret_cast<const unsigned char*>(&secret), &point);
    ge_p3_tobytes(reinterpret_cast<unsigned char*>(&output), &point);
  }

  inline void generate_tx_proof(const Hash& prefix_hash, const PublicKey& R, const PublicKey& A, const PublicKey& D, const SecretKey& r, Signature& sig) {
    generate_signature(prefix_hash, R, r, sig);
  }

  inline void generate_keys(PublicKey &pub, SecretKey &sec) {
    crypto_ops::generate_keys(pub, sec);
  }

  inline bool check_key(const PublicKey &key) {
    return crypto_ops::check_key(key);
  }

  inline bool secret_key_to_public_key(const SecretKey &sec, PublicKey &pub) {
    return crypto_ops::secret_key_to_public_key(sec, pub);
  }

  inline bool generate_key_derivation(const PublicKey &key1, const SecretKey &key2, KeyDerivation &derivation) {
    return crypto_ops::generate_key_derivation(key1, key2, derivation);
  }

  inline bool derive_public_key(const KeyDerivation &derivation, size_t output_index,
    const PublicKey &base, PublicKey &derived_key) {
    return crypto_ops::derive_public_key(derivation, output_index, base, derived_key);
  }

  inline bool derive_secret_key(const KeyDerivation &derivation, size_t output_index,
    const SecretKey &base, SecretKey &derived_key) {
    return crypto_ops::derive_secret_key(derivation, output_index, base, derived_key);
  }

  inline bool generate_signature(const Hash &prefix_hash, const PublicKey &pub,
    const SecretKey &sec, Signature &sig) {
    return crypto_ops::generate_signature(prefix_hash, pub, sec, sig);
  }

  inline bool check_signature(const Hash &prefix_hash, const PublicKey &pub,
    const Signature &sig) {
    return crypto_ops::check_signature(prefix_hash, pub, sig);
  }

  inline void generate_key_image(const PublicKey &pub, const SecretKey &sec,
    KeyImage &image) {
    crypto_ops::generate_key_image(pub, sec, image);
  }

  inline void hash_data_to_ec(const uint8_t* data, std::size_t len, PublicKey& key) {
    crypto_ops::hash_data_to_ec(data, len, key);
  }

  inline void generate_ring_signature(const Hash &prefix_hash, const KeyImage &image,
    const PublicKey *const* pubs, std::size_t pubs_count,
    const SecretKey &sec, std::size_t sec_index,
    Signature* sig) {
    crypto_ops::generate_ring_signature(prefix_hash, image, pubs, pubs_count, sec, sec_index, sig);
  }

  inline bool check_ring_signature(const Hash &prefix_hash, const KeyImage &image,
    const PublicKey *const* pubs, std::size_t pubs_count,
    const Signature* sig) {
    return crypto_ops::check_ring_signature(prefix_hash, image, pubs, pubs_count, sig);
  }

} // namespace crypto

namespace cn {
  using crypto::PublicKey;
  using crypto::SecretKey;
  using crypto::KeyDerivation;
  using crypto::Signature;
  using crypto::KeyImage;
  using crypto::EllipticCurvePoint;
  using crypto::EllipticCurveScalar;
}
