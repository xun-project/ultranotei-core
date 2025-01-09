// Copyright (c) 2011-2017 The Cryptonote developers
// Copyright (c) 2014-2017 XDN developers
// Copyright (c) 2016-2017 BXC developers
// Copyright (c) 2017 UltraNote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <alloca.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "Common/Varint.h"
#include "crypto.h"
#include "hash.h"
#include "random.h"
#include "secure_memory.h"
#include "secure_random.h"

namespace crypto {

  using namespace std;

  mutex random_lock;

  static inline void random_scalar(EllipticCurveScalar &res) {
    SecureRandom::instance().generateBytes(reinterpret_cast<uint8_t*>(&res), 32);
  }

  static inline void hash_to_scalar(const void *data, size_t length, EllipticCurveScalar &res) {
    SecureMemory<uint8_t> h(32);
    cn_fast_hash(data, length, reinterpret_cast<Hash&>(h.data()[0]));
    sc_reduce32(reinterpret_cast<unsigned char*>(&res));
  }

  void crypto_ops::generate_keys(PublicKey &pub, SecureSecretKey &sec) {
    SecureMemory<uint8_t> scalar(32);
    random_scalar(*reinterpret_cast<EllipticCurveScalar*>(scalar.data()));
    sec.resize(32);
    memcpy(sec.data(), scalar.data(), 32);
    ge_p3 point;
    ge_scalarmult_base(&point, reinterpret_cast<unsigned char*>(scalar.data()));
    ge_p3_tobytes(reinterpret_cast<unsigned char*>(&pub), &point);
  }

  bool crypto_ops::check_key(const PublicKey &key) {
    ge_p3 point;
    return ge_frombytes_vartime(&point, reinterpret_cast<const unsigned char*>(&key)) == 0;
  }

  bool crypto_ops::secret_key_to_public_key(const SecureSecretKey &sec, PublicKey &pub) {
    ge_p3 point;
    if (sc_check(reinterpret_cast<const unsigned char*>(sec.data())) != 0) {
      return false;
    }
    ge_scalarmult_base(&point, reinterpret_cast<const unsigned char*>(sec.data()));
    ge_p3_tobytes(reinterpret_cast<unsigned char*>(&pub), &point);
    return true;
  }

  bool crypto_ops::generate_key_derivation(const PublicKey &key1, const SecureSecretKey &key2, SecureKeyDerivation &derivation) {
    ge_p3 point;
    ge_p2 point2;
    ge_p1p1 point3;
    assert(sc_check(reinterpret_cast<const unsigned char*>(key2.data())) == 0);
    if (ge_frombytes_vartime(&point, reinterpret_cast<const unsigned char*>(&key1)) != 0) {
      return false;
    }
    ge_scalarmult(&point2, reinterpret_cast<const unsigned char*>(key2.data()), &point);
    ge_mul8(&point3, &point2);
    ge_p1p1_to_p2(&point2, &point3);
    derivation.resize(32);
    ge_tobytes(reinterpret_cast<unsigned char*>(derivation.data()), &point2);
    return true;
  }

  void crypto_ops::derivation_to_scalar(const SecureKeyDerivation &derivation, size_t output_index, EllipticCurveScalar &res) {
    struct {
      SecureKeyDerivation derivation;
      varint output_index;
    } buf;
    buf.derivation = derivation;
    buf.output_index = output_index;
    hash_to_scalar(&buf, sizeof(buf), res);
  }

  bool crypto_ops::derive_public_key(const SecureKeyDerivation &derivation, size_t output_index,
    const PublicKey &base, PublicKey &derived_key) {
    EllipticCurveScalar scalar;
    ge_p3 point1;
    ge_p3 point2;
    ge_cached point3;
    ge_p1p1 point4;
    ge_p2 point5;
    if (ge_frombytes_vartime(&point1, reinterpret_cast<const unsigned char*>(&base)) != 0) {
      return false;
    }
    derivation_to_scalar(derivation, output_index, scalar);
    ge_scalarmult_base(&point2, reinterpret_cast<unsigned char*>(&scalar));
    ge_p3_to_cached(&point3, &point2);
    ge_add(&point4, &point1, &point3);
    ge_p1p1_to_p2(&point5, &point4);
    ge_tobytes(reinterpret_cast<unsigned char*>(&derived_key), &point5);
    return true;
  }

  void crypto_ops::derive_secret_key(const SecureKeyDerivation &derivation, size_t output_index,
    const SecureSecretKey &base, SecureSecretKey &derived_key) {
    EllipticCurveScalar scalar;
    assert(sc_check(reinterpret_cast<const unsigned char*>(base.data())) == 0);
    derivation_to_scalar(derivation, output_index, scalar);
    sc_add(reinterpret_cast<unsigned char*>(&derived_key),
      reinterpret_cast<const unsigned char*>(&base),
      reinterpret_cast<unsigned char*>(&scalar));
  }

  void crypto_ops::generate_signature(const Hash &prefix_hash, const PublicKey &pub,
    const SecureSecretKey &sec, SecureSignature &sig) {
    SecureMemory<uint8_t> buf(32);
    SecureMemory<uint8_t> k(32);
    ge_p3 tmp3;
    EllipticCurveScalar k_scalar;
    random_scalar(k_scalar);
    memcpy(k.data(), &k_scalar, 32);
    ge_scalarmult_base(&tmp3, reinterpret_cast<unsigned char*>(k.data()));
    ge_p3_tobytes(reinterpret_cast<unsigned char*>(&sig), &tmp3);
    hash_to_scalar(&prefix_hash, sizeof(Hash), k_scalar);
    sc_mulsub(reinterpret_cast<unsigned char*>(&sig) + 32,
      reinterpret_cast<unsigned char*>(&k_scalar),
      reinterpret_cast<const unsigned char*>(sec.data()),
      reinterpret_cast<unsigned char*>(k.data()));
  }

  bool crypto_ops::check_signature(const Hash &prefix_hash, const PublicKey &pub,
    const SecureSignature &sig) {
    ge_p2 tmp2;
    ge_p3 tmp3;
    EllipticCurveScalar c;
    ge_p3 point;
    assert(check_key(pub));
    if (ge_frombytes_vartime(&point, reinterpret_cast<const unsigned char*>(&pub)) != 0) {
      return false;
    }
    hash_to_scalar(&prefix_hash, sizeof(Hash), c);
    if (sc_check(reinterpret_cast<const unsigned char*>(&sig) + 32) != 0 ||
      ge_frombytes_vartime(&tmp3, reinterpret_cast<const unsigned char*>(&sig)) != 0) {
      return false;
    }
    ge_double_scalarmult_base_vartime(&tmp2, reinterpret_cast<const unsigned char*>(&sig) + 32,
      &point, reinterpret_cast<const unsigned char*>(&c));
    ge_tobytes(reinterpret_cast<unsigned char*>(&c), &tmp2);
    return memcmp(&sig, &c, 32) == 0;
  }

  void crypto_ops::generate_key_image(const PublicKey &pub, const SecureSecretKey &sec,
    KeyImage &image) {
    ge_p3 point;
    ge_p2 point2;
    assert(sc_check(reinterpret_cast<const unsigned char*>(sec.data())) == 0);
    hash_to_ec(pub, point);
    ge_scalarmult(&point2, reinterpret_cast<const unsigned char*>(sec.data()), &point);
    ge_tobytes(reinterpret_cast<unsigned char*>(&image), &point2);
  }

  void crypto_ops::generate_ring_signature(const Hash &prefix_hash, const KeyImage &image,
    const PublicKey *const *pubs, size_t pubs_count,
    const SecureSecretKey &sec, size_t sec_index,
    SecureSignature *sig) {
    size_t i;
    ge_p3 image_unp;
    ge_dsmp image_pre;
    EllipticCurveScalar sum, k, h;
    rs_comm *const buf = reinterpret_cast<rs_comm *>(alloca(rs_comm_size(pubs_count)));
    assert(sec_index < pubs_count);
    assert(sc_check(reinterpret_cast<const unsigned char*>(sec.data())) == 0);
    if (ge_frombytes_vartime(&image_unp, reinterpret_cast<const unsigned char*>(&image)) != 0) {
      return;
    }
    ge_dsm_precomp(image_pre, &image_unp);
    sc_0(reinterpret_cast<unsigned char*>(&sum));
    buf->h = prefix_hash;
    for (i = 0; i < pubs_count; i++) {
      ge_p2 tmp2;
      ge_p3 tmp3;
      if (i == sec_index) {
        random_scalar(k);
        ge_scalarmult_base(&tmp3, reinterpret_cast<unsigned char*>(&k));
        ge_p3_tobytes(reinterpret_cast<unsigned char*>(&buf->ab[i].a), &tmp3);
        hash_to_ec(*pubs[i], tmp3);
        ge_scalarmult(&tmp2, reinterpret_cast<unsigned char*>(&k), &tmp3);
        ge_tobytes(reinterpret_cast<unsigned char*>(&buf->ab[i].b), &tmp2);
      } else {
        random_scalar(reinterpret_cast<EllipticCurveScalar&>(sig[i]));
        random_scalar(*reinterpret_cast<EllipticCurveScalar*>(
          reinterpret_cast<unsigned char*>(&sig[i]) + 32));
        if (ge_frombytes_vartime(&tmp3, reinterpret_cast<const unsigned char*>(pubs[i])) != 0) {
          return;
        }
        ge_double_scalarmult_base_vartime(&tmp2, reinterpret_cast<unsigned char*>(&sig[i]),
          &tmp3, reinterpret_cast<unsigned char*>(&sig[i]) + 32);
        ge_tobytes(reinterpret_cast<unsigned char*>(&buf->ab[i].a), &tmp2);
        hash_to_ec(*pubs[i], tmp3);
        ge_double_scalarmult_precomp_vartime(&tmp2, reinterpret_cast<unsigned char*>(&sig[i]) + 32,
          &tmp3, reinterpret_cast<unsigned char*>(&sig[i]), image_pre);
        ge_tobytes(reinterpret_cast<unsigned char*>(&buf->ab[i].b), &tmp2);
        sc_add(reinterpret_cast<unsigned char*>(&sum), reinterpret_cast<unsigned char*>(&sum),
          reinterpret_cast<unsigned char*>(&sig[i]));
      }
    }
    hash_to_scalar(buf, rs_comm_size(pubs_count), h);
    sc_sub(reinterpret_cast<unsigned char*>(&sig[sec_index]), reinterpret_cast<unsigned char*>(&h),
      reinterpret_cast<unsigned char*>(&sum));
    sc_mulsub(reinterpret_cast<unsigned char*>(&sig[sec_index]) + 32,
      reinterpret_cast<unsigned char*>(&sig[sec_index]),
      reinterpret_cast<const unsigned char*>(sec.data()),
      reinterpret_cast<unsigned char*>(&k));
  }

  bool crypto_ops::check_ring_signature(const Hash &prefix_hash, const KeyImage &image,
    const PublicKey *const *pubs, size_t pubs_count,
    const SecureSignature *sig) {
    size_t i;
    ge_p3 image_unp;
    ge_dsmp image_pre;
    EllipticCurveScalar sum, h;
    rs_comm *const buf = reinterpret_cast<rs_comm *>(alloca(rs_comm_size(pubs_count)));
    if (ge_frombytes_vartime(&image_unp, reinterpret_cast<const unsigned char*>(&image)) != 0) {
      return false;
    }
    ge_dsm_precomp(image_pre, &image_unp);
    sc_0(reinterpret_cast<unsigned char*>(&sum));
    buf->h = prefix_hash;
    for (i = 0; i < pubs_count; i++) {
      ge_p2 tmp2;
      ge_p3 tmp3;
      if (sc_check(reinterpret_cast<const unsigned char*>(&sig[i])) != 0 ||
        sc_check(reinterpret_cast<const unsigned char*>(&sig[i]) + 32) != 0) {
        return false;
      }
      if (ge_frombytes_vartime(&tmp3, reinterpret_cast<const unsigned char*>(pubs[i])) != 0) {
        return false;
      }
      ge_double_scalarmult_base_vartime(&tmp2, reinterpret_cast<const unsigned char*>(&sig[i]),
        &tmp3, reinterpret_cast<const unsigned char*>(&sig[i]) + 32);
      ge_tobytes(reinterpret_cast<unsigned char*>(&buf->ab[i].a), &tmp2);
      hash_to_ec(*pubs[i], tmp3);
      ge_double_scalarmult_precomp_vartime(&tmp2, reinterpret_cast<const unsigned char*>(&sig[i]) + 32,
        &tmp3, reinterpret_cast<const unsigned char*>(&sig[i]), image_pre);
      ge_tobytes(reinterpret_cast<unsigned char*>(&buf->ab[i].b), &tmp2);
      sc_add(reinterpret_cast<unsigned char*>(&sum), reinterpret_cast<unsigned char*>(&sum),
        reinterpret_cast<const unsigned char*>(&sig[i]));
    }
    hash_to_scalar(buf, rs_comm_size(pubs_count), h);
    sc_sub(reinterpret_cast<unsigned char*>(&h), reinterpret_cast<unsigned char*>(&h),
      reinterpret_cast<unsigned char*>(&sum));
    return sc_isnonzero(reinterpret_cast<unsigned char*>(&h)) == 0;
  }

} // namespace crypto
