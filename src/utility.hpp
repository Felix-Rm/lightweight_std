#pragma once

#ifdef ARDUINO
#    include "Arduino.h"
#else
#    include <sys/types.h>

#    include <cstddef>
#    include <cstdint>
#endif

/*
    FUNCTIONS
*/

#include "impl/move.hpp"

namespace lw_std {

// swap (1) https://en.cppreference.com/w/cpp/algorithm/swap
template <typename T>
constexpr void swap(T& a, T& b) noexcept {
    T temp = lw_std::move(a);
    a = lw_std::move(b);
    b = lw_std::move(temp);
}

// FIXME: swap (2) https://en.cppreference.com/w/cpp/algorithm/swap

}  // namespace lw_std

/*
    CLASSES
*/

#include "impl/pair.hpp"