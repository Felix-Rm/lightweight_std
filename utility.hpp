#pragma once

#ifdef ARDUINO
#    include "Arduino.h"
#else
#    include <sys/types.h>

#    include <cstddef>
#    include <cstdint>
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
    T first;
    U second;

    pair() = default;

    pair(const T& t, const U& u)
        : first(t), second(u){};

    // FIXME: Meant for compatibility with std::pair, but may match any type with "pair" syntax (which may not be a bad thing?)
    template <template <typename, typename> class S, typename A, typename B>
    pair(const S<A, B>& other) {
        first = other.first;
        second = other.second;
    }
};

// FIXME: These operators are meant for compatibility with std::pair, but they may catch more types because of the generic template
template <template <typename, typename> class T, template <typename, typename> class U, typename A, typename B, typename C, typename D>
constexpr bool operator==(const T<A, B>& lhs, const U<C, D>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <template <typename, typename> class T, template <typename, typename> class U, typename A, typename B, typename C, typename D>
constexpr bool operator!=(const T<A, B>& lhs, const U<C, D>& rhs) {
    return !operator==(lhs, rhs);
}

// lw_std implementation of std::swap
template <typename T>
void swap(T& a, T& b) {
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

#ifndef ARDUINO
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
#endif
}  // namespace lw_std