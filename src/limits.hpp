// limits header https://en.cppreference.com/w/cpp/header/limits
#pragma once

#include "utility.hpp"

namespace lw_std {

/*
    DECLARATIONS
*/

// numeric_limits https://en.cppreference.com/w/cpp/types/numeric_limits
template <typename T>
class numeric_limits {
   public:
    // max https://en.cppreference.com/w/cpp/types/numeric_limits/max
    static constexpr T max() noexcept;

    // min https://en.cppreference.com/w/cpp/types/numeric_limits/min
    static constexpr T min() noexcept;
};

template <>
class numeric_limits<uint8_t> {
   public:
    [[nodiscard]] static constexpr uint8_t max() noexcept {
        return 0b11111111u;
    }

    [[nodiscard]] static constexpr uint8_t min() noexcept {
        return 0;
    }
};

template <>
class numeric_limits<uint16_t> {
   public:
    [[nodiscard]] static constexpr uint16_t max() noexcept {
        return 0b11111111'11111111u;
    }

    [[nodiscard]] static constexpr uint16_t min() noexcept {
        return 0;
    }
};

template <>
class numeric_limits<uint32_t> {
   public:
    [[nodiscard]] static constexpr uint32_t max() noexcept {
        return 0b11111111'11111111'11111111'11111111u;
    }

    [[nodiscard]] static constexpr uint32_t min() noexcept {
        return 0;
    }
};

template <>
class numeric_limits<uint64_t> {
   public:
    [[nodiscard]] static constexpr uint64_t max() noexcept {
        return 0b11111111'11111111'11111111'11111111'11111111'11111111'11111111'11111111u;
    }

    [[nodiscard]] static constexpr uint64_t min() noexcept {
        return 0;
    }
};

}  // namespace lw_std