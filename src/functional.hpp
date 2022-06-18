#pragma once

#include "string.hpp"
#include "utility.hpp"

namespace lw_std {
// functional header https://en.cppreference.com/w/cpp/header/functional

/*
    CLASSES
*/

/*
    Comparisons
*/

// equal_to https://en.cppreference.com/w/cpp/utility/functional/equal_to
template <class T = void>
struct equal_to {
    constexpr bool operator()(const T& lhs, const T& rhs) {
        return lhs == rhs;
    }
};

/*
    Hashing
*/

// hash https://en.cppreference.com/w/cpp/utility/hash
template <typename T>
struct hash {
    typedef T argument_type;
    typedef size_t result_type;

    size_t operator()(const T& key) const {
        return key;
    }
};

// FIXME: this implementation ignores the last n bytes that don't fit into 4-byte chunks
template <>
struct hash<lw_std::string> {
    typedef lw_std::string argument_type;
    typedef size_t result_type;

    size_t operator()(const lw_std::string& key) const {
        // NOTE:: key.data() is not available on Arduino
        return hash_bytes(key.c_str(), key.length());
    }

   private:
    size_t hash_bytes(const void* ptr, size_t len) const {
        const size_t m = 0x5bd1e995;
        size_t hash = m;

        auto mix = [&](uint32_t val) {
            val *= m;
            val ^= val >> 23;
            val *= m;
            hash *= m;
            hash ^= val;
        };

        while (len >= 4) {
            len -= sizeof(uint32_t);
            mix(*((uint32_t*)ptr + len));
        }

        // FIXME:
        // mix(*((uint32_t*) ptr));

        hash ^= hash >> 11;
        return hash;
    };
};

}  // namespace lw_std
