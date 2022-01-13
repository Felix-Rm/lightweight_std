#pragma once

#ifdef ARDUINO
#include "Arduino.h"
#else
#include <cstddef>
#include <cstdint>
#endif

namespace lw_std {

// lw_std implementation of remove_reference (used for lw_std::move)
template <typename T>
struct remove_reference { typedef T type; };
template <typename T>
struct remove_reference<T&> { typedef T type; };
template <typename T>
struct remove_reference<T&&> { typedef T type; };

// lw_std implementation of std::move
template <typename T>
constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept {
    return (typename remove_reference<T>::type &&)(arg);
};

// lw_std implementation of std::pair
template <typename T, typename U>
class pair {
   public:
    T first{};
    U second{};
};

// lw_std implementation of std::swap
template <typename T>
void swap(T& a, T& b) {
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

// lw_std implementation of std::max
template <typename T>
constexpr T max(const T& a, const T& b) {
    return a > b ? a : b;
}

// lw_std implementation of std::min
template <typename T>
constexpr T min(const T& a, const T& b) {
    return a < b ? a : b;
}
}  // namespace lw_std