#pragma once

#ifdef ARDUINO
#include "Arduino.hpp"
#undef max
#undef min
#else
#include <cstdint>
#endif

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
    static constexpr uint8_t max() { return 255; };
    static constexpr uint8_t min() { return 0; };
};

// lw_std implementation of std::limits for uint16_t
template <>
class numeric_limits<uint16_t> {
   public:
    static constexpr uint16_t max() { return 65535; };
    static constexpr uint16_t min() { return 0; };
};

}  // namespace lw_std