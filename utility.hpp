#pragma once

#ifdef ARDUINO
#    include "Arduino.h"
#else
#    include <sys/types.h>

#    include <cstddef>
#    include <cstdint>
#endif

namespace lw_std {

/*
    Functions
*/

// swap (1) https://en.cppreference.com/w/cpp/algorithm/swap
template <typename T>
constexpr void swap(T& a, T& b) noexcept {
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

// FIXME: swap (2) https://en.cppreference.com/w/cpp/algorithm/swap

// remove_reference for move https://en.cppreference.com/w/cpp/utility/move
template <typename T>
struct remove_reference { typedef T type; };
template <typename T>
struct remove_reference<T&> { typedef T type; };
template <typename T>
struct remove_reference<T&&> { typedef T type; };

// move https://en.cppreference.com/w/cpp/utility/move
template <typename T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept {
    return (typename remove_reference<T>::type &&)(t);
};

// swap (1) https://en.cppreference.com/w/cpp/algorithm/swap
template <typename T>
constexpr void swap(T& a, T& b) noexcept {
    T temp = lw_std::move(a);
    a = lw_std::move(b);
    b = lw_std::move(temp);
}

// FIXME: swap (2) https://en.cppreference.com/w/cpp/algorithm/swap

/*
    Classes
*/

// pair https://en.cppreference.com/w/cpp/utility/pair
template <typename T1, typename T2>
struct pair {
    /*
       Member types
    */

    typedef T1 first_type;
    typedef T2 second_type;

    /*
       Member objects
    */

    first_type first;
    second_type second;

    /*
       Member functions
    */

    // (constructor) (1) https://en.cppreference.com/w/cpp/utility/pair/pair
    constexpr pair() noexcept = default;

    // (constructor) (2) https://en.cppreference.com/w/cpp/utility/pair/pair
    constexpr pair(const first_type& t, const second_type& u) noexcept
        : first(t), second(u){};

    // (constructor) (3) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair(U1&& t, U2&& u) noexcept
        : first(lw_std::move(t)), second(lw_std::move(u)){};

    // (constructor) (4/5/8) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair(const pair<U1, U2>& p) noexcept
        : first(p.first), second(p.second){};

    // (constructor) (6/7/10) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair(const pair<U1, U2>&& p) noexcept
        : first(move(p.first)), second(move(p.second)){};

    // FIXME: (constructor) (9) https://en.cppreference.com/w/cpp/utility/pair/pair

    // FIXME: Meant for compatibility with std::pair, but may match any type with "pair" syntax (which may not be a bad thing?)
    template <template <typename, typename> typename S, typename A, typename B>
    pair(const S<A, B>& other) {
        first = other.first;
        second = other.second;
    }

    // operator= (1/3) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair& operator=(const pair<U1, U2>& other) noexcept {
        first = other.first;
        second = other.second;
    };

    // operator= (2/4) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr const pair& operator=(const pair<U1, U2>& other) const noexcept {
        first = other.first;
        second = other.second;
    };

    // operator= (5/7) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair& operator=(pair<U1, U2>&& other) noexcept {
        first = lw_std::move(other.first);
        second = lw_std::move(other.second);
    };

    // operator= (6/8) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr const pair& operator=(pair<U1, U2>&& other) const noexcept {
        first = lw_std::move(other.first);
        second = lw_std::move(other.second);
    };

    // swap (1/2) https://en.cppreference.com/w/cpp/utility/pair/swap
    constexpr void swap(const pair& other) noexcept {
        swap(*this, other);
    }
};

/*
    Non-member functions
*/

// make_pair https://en.cppreference.com/w/cpp/utility/pair/make_pair
template <typename T1, typename T2>
constexpr pair<T1, T2> make_pair(T1 t, T2 u) noexcept {
    return {lw_std::move(t), lw_std::move(u)};
}

// make_pair https://en.cppreference.com/w/cpp/utility/pair/make_pair
template <typename T1, typename T2>
constexpr pair<T1, T2> make_pair(T1&& t, T2&& u) noexcept {
    return {lw_std::move(t), lw_std::move(u)};
}

// operator== (1)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp
template <typename T1, typename T2>
constexpr bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) noexcept {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

// operator!= (2)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp
template <typename T1, typename T2>
constexpr bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) noexcept {
    return !operator==(lhs, rhs);
}

// operator< (3)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp
template <typename T1, typename T2>
constexpr bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) noexcept {
    if (synthesized_cmp_equal(lhs.first, rhs.first))
        return synthesized_cmp_less(lhs.second, rhs.second);
    return synthesized_cmp_less(lhs.first, rhs.first);
}

// operator<= (4)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp
template <typename T1, typename T2>
constexpr bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) noexcept {
    if (synthesized_cmp_equal(lhs.first, rhs.first))
        return synthesized_cmp_less_equal(lhs.second, rhs.second);
    return synthesized_cmp_less_equal(lhs.first, rhs.first);
}

// operator> (5)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp
template <typename T1, typename T2>
constexpr bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) noexcept {
    if (synthesized_cmp_equal(lhs.first, rhs.first))
        return synthesized_cmp_greater(lhs.second, rhs.second);
    return synthesized_cmp_greater(lhs.first, rhs.first);
}

// operator> (6)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp
template <typename T1, typename T2>
constexpr bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) noexcept {
    if (synthesized_cmp_equal(lhs.first, rhs.first))
        return synthesized_cmp_greater_equal(lhs.second, rhs.second);
    return synthesized_cmp_greater_equal(lhs.first, rhs.first);
}

// FIXME: operator<=> (7)  https://en.cppreference.com/w/cpp/utility/pair/operator_cmp

// NOTE: These operators are meant for compatibility with std::pair, but they may catch more types because of the generic template
template <template <typename, typename> typename T, template <typename, typename> typename U, typename A, typename B, typename C, typename D>
constexpr bool operator==(const T<A, B>& lhs, const U<C, D>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <template <typename, typename> typename T, template <typename, typename> typename U, typename A, typename B, typename C, typename D>
constexpr bool operator!=(const T<A, B>& lhs, const U<C, D>& rhs) {
    return !operator==(lhs, rhs);
}

// NOTE: no extra specialization: swap https://en.cppreference.com/w/cpp/utility/pair/swap2

// FIXME: get (1-12) https://en.cppreference.com/w/cpp/utility/pair/get

}  // namespace lw_std