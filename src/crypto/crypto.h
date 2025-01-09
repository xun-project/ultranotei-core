#pragma once

#include <cstddef>
#include <limits>
#include <mutex>
#include <type_traits>
#include <vector>

#include "CryptoTypes.h"
#include "hash.h"
#include "secure_memory.h"

namespace crypto {

  using SecureSecretKey = SecureMemory<uint8_t>;
  using SecureKeyDerivation = SecureMemory<uint8_t>;
  using SecureSignature = SecureMemory<uint8_t>;

  struct EllipticCurvePoint {
    uint8_t data[32];
  };

  struct EllipticCurveScalar {
    uint8_t data[32];
  };

  struct PublicKey : public EllipticCurvePoint {};
  struct KeyImage : public EllipticCurvePoint {};

  class crypto_ops {
    crypto_ops();
    crypto_ops(const crypto_ops&);
    void operator=(const crypto_ops&);
    ~crypto_ops();

    static void generate_keys(PublicKey &pub, SecureSecretKey &sec);
    static bool check_key(const PublicKey &key);
    static bool secret_key_to_public_key(const SecureSecretKey &sec, PublicKey &pub);
    static bool generate_key_derivation(const PublicKey &key1, const SecureSecretKey &key2, SecureKeyDerivation &derivation);
    static void derivation_to_scalar(const SecureKeyDerivation &derivation, size_t output_index, EllipticCurveScalar &res);
    static bool derive_public_key(const SecureKeyDerivation &derivation, size_t output_index, const PublicKey &base, PublicKey &derived_key);
    static void derive_secret_key(const SecureKeyDerivation &derivation, size_t output_index, const SecureSecretKey &base, SecureSecretKey &derived_key);
    static void generate_signature(const Hash &prefix_hash, const PublicKey &pub, const SecureSecretKey &sec, SecureSignature &sig);
    static bool check_signature(const Hash &prefix_hash, const PublicKey &pub, const SecureSignature &sig);
    static void generate_key_image(const PublicKey &pub, const SecureSecretKey &sec, KeyImage &image);
    static void generate_ring_signature(const Hash &prefix_hash, const KeyImage &image,
      const PublicKey *const *pubs, size_t pubs_count,
      const SecureSecretKey &sec, size_t sec_index,
      SecureSignature *sig);
    static bool check_ring_signature(const Hash &prefix_hash, const KeyImage &image,
      const PublicKey *const *pubs, size_t pubs_count,
      const SecureSignature *sig);

    friend class CryptoOps;
  };

  class CryptoOps {
  public:
    static void generate_keys(PublicKey &pub, SecureSecretKey &sec) {
      crypto_ops::generate_keys(pub, sec);
    }

    static bool check_key(const PublicKey &key) {
      return crypto_ops::check_key(key);
    }

    static bool secret_key_to_public_key(const SecureSecretKey &sec, PublicKey &pub) {
      return crypto_ops::secret_key_to_public_key(sec, pub);
    }

    static bool generate_key_derivation(const PublicKey &key1, const SecureSecretKey &key2, SecureKeyDerivation &derivation) {
      return crypto_ops::generate_key_derivation(key1, key2, derivation);
    }

    static void derivation_to_scalar(const SecureKeyDerivation &derivation, size_t output_index, EllipticCurveScalar &res) {
      crypto_ops::derivation_to_scalar(derivation, output_index, res);
    }

    static bool derive_public_key(const SecureKeyDerivation &derivation, size_t output_index, const PublicKey &base, PublicKey &derived_key) {
      return crypto_ops::derive_public_key(derivation, output_index, base, derived_key);
    }

    static void derive_secret_key(const SecureKeyDerivation &derivation, size_t output_index, const SecureSecretKey &base, SecureSecretKey &derived_key) {
      crypto_ops::derive_secret_key(derivation, output_index, base, derived_key);
    }

    static void generate_signature(const Hash &prefix_hash, const PublicKey &pub, const SecureSecretKey &sec, SecureSignature &sig) {
      crypto_ops::generate_signature(prefix_hash, pub, sec, sig);
    }

    static bool check_signature(const Hash &prefix_hash, const PublicKey &pub, const SecureSignature &sig) {
      return crypto_ops::check_signature(prefix_hash, pub, sig);
    }

    static void generate_key_image(const PublicKey &pub, const SecureSecretKey &sec, KeyImage &image) {
      crypto_ops::generate_key_image(pub, sec, image);
    }

    static void generate_ring_signature(const Hash &prefix_hash, const KeyImage &image,
      const PublicKey *const *pubs, size_t pubs_count,
      const SecureSecretKey &sec, size_t sec_index,
      SecureSignature *sig) {
      crypto_ops::generate_ring_signature(prefix_hash, image, pubs, pubs_count, sec, sec_index, sig);
    }

    static bool check_ring_signature(const Hash &prefix_hash, const KeyImage &image,
      const PublicKey *const *pubs, size_t pubs_count,
      const SecureSignature *sig) {
      return crypto_ops::check_ring_signature(prefix_hash, image, pubs, pubs_count, sig);
    }
  };

} // namespace crypto

namespace cn {
  using crypto::SecureSecretKey;
  using crypto::SecureKeyDerivation;
  using crypto::SecureSignature;
  using crypto::PublicKey;
  using crypto::KeyImage;
  using crypto::EllipticCurvePoint;
  using crypto::EllipticCurveScalar;
}
