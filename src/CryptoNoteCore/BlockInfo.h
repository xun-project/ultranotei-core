#pragma once

#include "crypto/hash.h"

namespace cn {

struct BlockInfo {
    crypto::Hash id;
    uint32_t height;

    BlockInfo() : height(0) {}
    BlockInfo(const crypto::Hash& _id, uint32_t _height) : id(_id), height(_height) {}

    bool empty() const { return id == crypto::Hash(); }
};

} // namespace cn
