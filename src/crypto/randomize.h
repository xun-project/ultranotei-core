// Copyright (c) 2012-2016, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2019, The TurtleCoin Developers
// Copyright (c) 2019, The Karbo Developers
//
// This file is part of Karbo.
//
// Karbo is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Karbo is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Karbo. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <cstdint>
#include <mutex>
#include <random>

namespace crypto {

  inline std::mt19937_64& get_random_generator() {
    static thread_local std::mt19937_64 generator(std::random_device{}());
    return generator;
  }

  inline void randomize(void* data, size_t size) {
    static thread_local std::uniform_int_distribution<uint64_t> distribution;
    uint8_t* dataPtr = static_cast<uint8_t*>(data);
    
    while (size >= 8) {
      *reinterpret_cast<uint64_t*>(dataPtr) = distribution(get_random_generator());
      dataPtr += 8;
      size -= 8;
    }
    
    if (size > 0) {
      uint64_t random = distribution(get_random_generator());
      std::memcpy(dataPtr, &random, size);
    }
  }

  inline uint64_t rand() {
    static thread_local std::uniform_int_distribution<uint64_t> distribution;
    return distribution(get_random_generator());
  }

  inline uint64_t rand64() {
    return rand();
  }

  template<typename T>
  T rand() {
    static thread_local std::uniform_int_distribution<T> distribution;
    return distribution(get_random_generator());
  }

  template<>
  inline uint8_t rand<uint8_t>() {
    return static_cast<uint8_t>(rand() & 0xFF);
  }

  template<>
  inline int8_t rand<int8_t>() {
    return static_cast<int8_t>(rand() & 0xFF);
  }

  template<>
  inline char rand<char>() {
    return static_cast<char>(rand() & 0xFF);
  }

}  // namespace crypto
