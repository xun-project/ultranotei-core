#pragma once

#include <string>
#include <system_error>

namespace cn {
namespace error {

enum class CoreErrorCode {
  NOT_INITIALIZED = 1,
  ALREADY_INITIALIZED = 2,
  CORRUPTED_BLOCKCHAIN = 3,
  INVALID_ARGUMENT = 4,
  NOT_FOUND = 5,
  OUT_OF_MEMORY = 6,
  INTERNAL_ERROR = 7,
  TX_TOO_BIG = 8,
  TX_TOO_MANY_INS = 9,
  TX_TOO_MANY_OUTS = 10,
  TX_PUBLIC_KEY_TOO_BIG = 11,
  TX_EXTRA_TOO_BIG = 12,
  TX_EXTRA_PARSE_ERROR = 13,
  TX_INVALID_INPUT_AMOUNT = 14,
  TX_INVALID_OUTPUT_AMOUNT = 15,
  TX_AMOUNT_OVERFLOW = 16,
  TX_FROM_FUTURE = 17,
  TX_EXPIRED = 18,
  TX_INVALID_REFERENCE = 19,
  TX_KEYS_MISMATCH = 20,
  TX_WRONG_VERSION = 21,
  TX_DOUBLE_SPEND = 22,
  TX_KEY_IMAGE_USED = 23,
  TX_MISSING_INPUT = 24,
  TX_MISSING_RING_SIGNATURE = 25,
  TX_INVALID_RING_SIGNATURE = 26,
  TX_WRONG_ORDER = 27,
  TX_POOL_FULL = 28,
  TX_CANCELLED = 29
};

// Custom category:
class CoreErrorCategory : public std::error_category {
public:
  static CoreErrorCategory INSTANCE;

  virtual const char* name() const throw() {
    return "CoreErrorCategory";
  }

  virtual std::error_condition default_error_condition(int ev) const throw() {
    return std::error_condition(ev, *this);
  }

  virtual std::string message(int ev) const {
    CoreErrorCode code = static_cast<CoreErrorCode>(ev);

    switch (code) {
      case CoreErrorCode::NOT_INITIALIZED:
        return "Object was not initialized";
      case CoreErrorCode::ALREADY_INITIALIZED:
        return "Object has been already initialized";
      case CoreErrorCode::CORRUPTED_BLOCKCHAIN:
        return "Blockchain storage is corrupted";
      case CoreErrorCode::INVALID_ARGUMENT:
        return "Invalid argument";
      case CoreErrorCode::NOT_FOUND:
        return "Requested object was not found";
      case CoreErrorCode::OUT_OF_MEMORY:
        return "Not enough memory";
      case CoreErrorCode::INTERNAL_ERROR:
        return "Internal error";
      case CoreErrorCode::TX_TOO_BIG:
        return "Transaction is too big";
      case CoreErrorCode::TX_TOO_MANY_INS:
        return "Transaction has too many inputs";
      case CoreErrorCode::TX_TOO_MANY_OUTS:
        return "Transaction has too many outputs";
      case CoreErrorCode::TX_PUBLIC_KEY_TOO_BIG:
        return "Transaction public key is too big";
      case CoreErrorCode::TX_EXTRA_TOO_BIG:
        return "Transaction extra is too big";
      case CoreErrorCode::TX_EXTRA_PARSE_ERROR:
        return "Couldn't parse transaction extra";
      case CoreErrorCode::TX_INVALID_INPUT_AMOUNT:
        return "Invalid input amount";
      case CoreErrorCode::TX_INVALID_OUTPUT_AMOUNT:
        return "Invalid output amount";
      case CoreErrorCode::TX_AMOUNT_OVERFLOW:
        return "Amount overflow";
      case CoreErrorCode::TX_FROM_FUTURE:
        return "Transaction timestamp is from the future";
      case CoreErrorCode::TX_EXPIRED:
        return "Transaction is expired";
      case CoreErrorCode::TX_INVALID_REFERENCE:
        return "Invalid transaction reference";
      case CoreErrorCode::TX_KEYS_MISMATCH:
        return "Transaction keys mismatch";
      case CoreErrorCode::TX_WRONG_VERSION:
        return "Wrong transaction version";
      case CoreErrorCode::TX_DOUBLE_SPEND:
        return "Double spending attempt";
      case CoreErrorCode::TX_KEY_IMAGE_USED:
        return "Key image is already spent";
      case CoreErrorCode::TX_MISSING_INPUT:
        return "Missing input";
      case CoreErrorCode::TX_MISSING_RING_SIGNATURE:
        return "Missing ring signature";
      case CoreErrorCode::TX_INVALID_RING_SIGNATURE:
        return "Invalid ring signature";
      case CoreErrorCode::TX_WRONG_ORDER:
        return "Wrong transaction order";
      case CoreErrorCode::TX_POOL_FULL:
        return "Transaction pool is full";
      case CoreErrorCode::TX_CANCELLED:
        return "Transaction cancelled";
      default:
        return "Unknown error";
    }
  }
};

inline std::error_code make_error_code(CoreErrorCode e) {
  return std::error_code(static_cast<int>(e), CoreErrorCategory::INSTANCE);
}

} // namespace error
} // namespace cn

namespace std {

template <>
struct is_error_code_enum<cn::error::CoreErrorCode>: public true_type {};

}
