#pragma once

#include "utility.hpp"

namespace lw_std {

// lw_std implementation of std::limits
template <typename T>
class numeric_limits {
   public:
    static constexpr T max();
    static constexpr T min();
};

// lw_std implementation of std::limits for uint8_t
template <>
class numeric_limits<uint8_t> {
   public:
    static constexpr uint8_t max() {
        return 255;
    };

    static constexpr uint8_t min() {
        return 0;
    };
};

// lw_std implementation of std::limits for uint16_t
template <>
class numeric_limits<uint16_t> {
   public:
    static constexpr uint16_t max() {
        return 65535;
    };

    static constexpr uint16_t min() {
        return 0;
    };
};

// lw_std implementation of std::limits for uint32_t
template <>
class numeric_limits<uint32_t> {
   public:
    static constexpr uint32_t max() {
        return 4294967295L;
    };

    static constexpr uint32_t min() {
        return 0;
    };
};

// lw_std implementation of std::limits for uint64_t
template <>
class numeric_limits<uint64_t> {
   public:
    static constexpr uint64_t max() {
        return 18446744073709551615UL;
    };

    static constexpr uint64_t min() {
        return 0;
    };
};

}  // namespace lw_std