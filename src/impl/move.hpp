#pragma once

namespace lw_std {

// remove_reference for move and forward https://en.cppreference.com/w/cpp/utility/move
template <typename T>
struct remove_reference { typedef T type; };
template <typename T>
struct remove_reference<T&> { typedef T type; };
template <typename T>
struct remove_reference<T&&> { typedef T type; };

// forward https://en.cppreference.com/w/cpp/utility/forward
template <typename T>
[[nodiscard]] constexpr T&& forward(typename remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
[[nodiscard]] constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept {
    return static_cast<T&&>(t);
}

// move https://en.cppreference.com/w/cpp/utility/move
template <typename T>
[[nodiscard]] constexpr typename remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(t);
}

}  // namespace lw_std
