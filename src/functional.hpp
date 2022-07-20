// functional header https://en.cppreference.com/w/cpp/header/functional
#pragma once

#include "string.hpp"
#include "utility.hpp"

namespace lw_std {

/*
    CLASSES
*/

/*
    Comparisons
*/

// equal_to https://en.cppreference.com/w/cpp/utility/functional/equal_to
template <class T = void>
struct equal_to {
    [[nodiscard]] constexpr bool operator()(const T& lhs, const T& rhs) const {
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

    [[nodiscard]] constexpr result_type operator()(const argument_type& key) const {
        return static_cast<result_type>(key);
    }
};

// FIXME: this implementation ignores the last n bytes that don't fit into 4-byte chunks
template <>
struct hash<lw_std::string> {
    typedef lw_std::string argument_type;
    typedef size_t result_type;

    [[nodiscard]] size_t operator()(const argument_type& key) const {
        // NOTE:: key.data() is not available on arduino
        return hash_bytes(key.c_str(), key.length());
    }

   private:
    [[nodiscard]] constexpr size_t hash_bytes(const char* ptr, size_t len) const {
        const size_t m = 0x5bd1e995;
        size_t current_hash = m;

        auto mix = [&](uint32_t val) {
            val *= m;
            val ^= val >> 23;
            val *= m;
            current_hash *= m;
            current_hash ^= val;
        };

        while (len >= 4) {
            len -= sizeof(uint32_t);
            mix(*reinterpret_cast<const uint32_t*>(ptr + len));
        }

        // FIXME:
        // mix(*((uint32_t*) ptr));

        current_hash ^= current_hash >> 11;
        return current_hash;
    }
};

}  // namespace lw_std
