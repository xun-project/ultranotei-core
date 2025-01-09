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

#include <algorithm>
#include <random>
#include <type_traits>

namespace randomize
{
    /**
     * Obtain a random seed
     */
    static thread_local std::random_device device;

    /**
     * Generator, seeded with the random device
     */
    static thread_local std::mt19937 gen(device());

    /**
     * Obtain the generator used internally. Helpful for passing to functions
     * like std::shuffle.
     */
    inline std::mt19937 generator()
    {
        return gen;
    }

    /**
     * Generate n random bytes (uint8_t), and place them in *result. Result should be large
     * enough to contain the bytes.
     */
    inline void randomBytes(size_t n, uint8_t *result)
    {
        for (size_t i = 0; i < n; i++)
        {
            result[i] = static_cast<uint8_t>(random_value<uint8_t>(0, std::numeric_limits<uint8_t>::max()));
        }
    }

    /**
     * Generate n random bytes (uint8_t), and return them in a vector.
     */
    inline std::vector<uint8_t> randomBytes(size_t n)
    {
        std::vector<uint8_t> result;

        result.reserve(n);

        for (size_t i = 0; i < n; i++)
        {
            result.push_back(static_cast<uint8_t>(random_value<uint8_t>(0, std::numeric_limits<uint8_t>::max())));
        }

        return result;
    }

    /**
     * Generate a random value of the type specified, in the full range of the
     * type
     */
    template <typename T>
    T randomValue()
    {
        return random_value<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    /**
     * Generate a random value of the type specified, in the range [min, max]
     * Note that both min, and max, are included in the results. Therefore,
     * randomValue(0, 100), will generate numbers between 0 and 100.
     *
     * Note that min must be <= max, or undefined behaviour will occur.
     */
    template <typename T>
    T randomValue(T min, T max)
    {
        return random_value<T>(min, max);
    }

    /**
     * Generate a random value of the type specified, in the range [min, max]
     * Note that both min, and max, are included in the results. Therefore,
     * randomValue(0, 100), will generate numbers between 0 and 100.
     *
     * Note that min must be <= max, or undefined behaviour will occur.
     */
    template <typename T>
    T random_value(T min, T max)
    {
        std::lock_guard<std::mutex> lock(random_lock);
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen);
    }

    /**
     * Obtain the generator used internally. Helpful for passing to functions
     * like std::shuffle.
     */
    inline std::mutex random_lock;
    inline std::random_device random_engine;

    /**
     * Shuffle the range [first, last) using the random engine
     */
    template <typename Iter>
    void shuffle(Iter first, Iter last)
    {
        size_t count = std::distance(first, last);
        if (count < 2) {
            return;
        }

        std::lock_guard<std::mutex> lock(random_lock);
        for (size_t i = count - 1; i > 0; --i) {
            size_t j = random_value<size_t>(0, i);
            if (i != j) {
                std::swap(first[i], first[j]);
            }
        }
    }
}
