#pragma once

#include "move.hpp"

#ifdef LWSTD_BUILD_STD_COMPATIBILITY
#    include <utility>
#endif

namespace lw_std {

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
        : first(lw_std::move(t)), second(lw_std::move(u)) {}

    // (constructor) (4/5/8) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair(const pair<U1, U2>& p) noexcept
        : first(p.first), second(p.second) {}

    // (constructor) (6/7/10) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair(const pair<U1, U2>&& p) noexcept
        : first(lw_std::move(p.first)), second(lw_std::move(p.second)) {}

    // FIXME: (constructor) (9) https://en.cppreference.com/w/cpp/utility/pair/pair

#ifdef LWSTD_BUILD_STD_COMPATIBILITY
    template <typename U1 = T1, typename U2 = T2>
    pair(const ::std::pair<U1, U2>& p) noexcept
        : first(p.first), second(p.second) {}
#endif

    // operator= (1/3) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair& operator=(const pair<U1, U2>& other) noexcept {
        first = other.first;
        second = other.second;
    }

    // operator= (2/4) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr const pair& operator=(const pair<U1, U2>& other) const noexcept {
        first = other.first;
        second = other.second;
    }

    // operator= (5/7) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr pair& operator=(pair<U1, U2>&& other) noexcept {
        first = lw_std::move(other.first);
        second = lw_std::move(other.second);
    }

    // operator= (6/8) https://en.cppreference.com/w/cpp/utility/pair/pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr const pair& operator=(pair<U1, U2>&& other) const noexcept {
        first = lw_std::move(other.first);
        second = lw_std::move(other.second);
    }

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

#ifdef LWSTD_BUILD_STD_COMPATIBILITY

#    define LWSTD_GENERATE_COMPAT_HELPER1(op_name)                                                 \
        template <typename T1, typename T2, typename U1, typename U2>                              \
        constexpr bool op_name(const ::std::pair<T1, T2>& lhs, const pair<U1, U2>& rhs) noexcept { \
            return op_name(pair<T1, T2>{lhs}, rhs);                                                \
        }

#    define LWSTD_GENERATE_COMPAT_HELPER2(op_name)                                                 \
        template <typename T1, typename T2, typename U1, typename U2>                              \
        constexpr bool op_name(const pair<T1, T2>& lhs, const ::std::pair<U1, U2>& rhs) noexcept { \
            return op_name(lhs, pair<U1, U2>{rhs});                                                \
        }

#    define LWSTD_GENERATE_COMPAT(op_name)     \
        LWSTD_GENERATE_COMPAT_HELPER1(op_name) \
        LWSTD_GENERATE_COMPAT_HELPER2(op_name)

LWSTD_GENERATE_COMPAT(operator==)
LWSTD_GENERATE_COMPAT(operator!=)
LWSTD_GENERATE_COMPAT(operator<)
LWSTD_GENERATE_COMPAT(operator>)
LWSTD_GENERATE_COMPAT(operator<=)
LWSTD_GENERATE_COMPAT(operator>=)

#    undef LWSTD_GENERATE_COMPAT_HELPER1
#    undef LWSTD_GENERATE_COMPAT_HELPER2
#    undef LWSTD_GENERATE_COMPAT

#endif

// NOTE: no extra specialization: swap https://en.cppreference.com/w/cpp/utility/pair/swap2

// FIXME: get (1-12) https://en.cppreference.com/w/cpp/utility/pair/get

}  // namespace lw_std