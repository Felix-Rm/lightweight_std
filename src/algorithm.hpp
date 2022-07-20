// algorithm header https://en.cppreference.com/w/cpp/header/algorithm
#pragma once

#include "utility.hpp"

namespace lw_std {

/*
    FUNCTIONS
*/

/*
    Non-standard
*/

template <typename T, typename U>
[[nodiscard]] constexpr bool synthesized_cmp_less(const T& t, const U& u) {
    return t < u;
}

template <typename T, typename U>
[[nodiscard]] constexpr bool synthesized_cmp_greater(const T& t, const U& u) {
    return u < t;
}

template <typename T, typename U>
[[nodiscard]] constexpr bool synthesized_cmp_equal(const T& t, const U& u) {
    return !(t < u) && !(u < t);
}

template <typename T, typename U>
[[nodiscard]] constexpr bool synthesized_cmp_not_equal(const T& t, const U& u) {
    return !synthesized_cmp_equal(t, u);
}

template <typename T, typename U>
[[nodiscard]] constexpr bool synthesized_cmp_less_equal(const T& t, const U& u) {
    return synthesized_cmp_less(t, u) || synthesized_cmp_equal(t, u);
}

template <typename T, typename U>
[[nodiscard]] constexpr bool synthesized_cmp_greater_equal(const T& t, const U& u) {
    return synthesized_cmp_greater(t, u) || synthesized_cmp_equal(t, u);
}

/*
    Non-modifying sequence operations
*/

// find (1) https://en.cppreference.com/w/cpp/algorithm/find
template <typename InputIt, typename T>
[[nodiscard]] constexpr InputIt find(InputIt first, InputIt last, const T& value) {
    for (; first != last; ++first)
        if (*first == value)
            return first;

    return last;
}

// FIXME: find (2) https://en.cppreference.com/w/cpp/algorithm/find

// find_if (3) https://en.cppreference.com/w/cpp/algorithm/find
template <typename InputIt, typename UnaryPredicate>
[[nodiscard]] constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first)
        if (p(*first))
            return first;

    return last;
}

// FIXME: find_if (4) https://en.cppreference.com/w/cpp/algorithm/find

// find_if_not (5) https://en.cppreference.com/w/cpp/algorithm/find
template <typename InputIt, typename UnaryPredicate>
[[nodiscard]] constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q) {
    for (; first != last; ++first)
        if (!q(*first))
            return first;

    return last;
}

// FIXME: find_if_not (6) https://en.cppreference.com/w/cpp/algorithm/find

/*
    Modifying sequence operations
*/

// common erase (specialization (1) reference for vector: https://en.cppreference.com/w/cpp/container/vector/erase2)
template <typename T, typename U>
constexpr typename T::size_type erase(T& c, const U& value) {
    auto old_size = c.size();
    remove(c.begin(), c.end(), value);
    return old_size - c.size();
}

// common erase_if (specialization (2) reference for vector: https://en.cppreference.com/w/cpp/container/vector/erase2)
template <typename T, typename Pred>
constexpr typename T::size_type erase_if(T& c, Pred pred) {
    auto old_size = c.size();
    remove(c.begin(), c.end(), pred);
    return old_size - c.size();
}

// remove (1) https://en.cppreference.com/w/cpp/algorithm/remove
template <typename ForwardIt, typename T>
constexpr ForwardIt remove(ForwardIt first, ForwardIt last, const T& value) {
    return remove_if(
        first, last, [value](const typename ForwardIt::reference compare_value) constexpr { return value == compare_value; });
}

// remove_if (2) https://en.cppreference.com/w/cpp/algorithm/remove
template <typename ForwardIt, typename UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p) {
    first = find_if(first, last, p);
    if (first != last)
        for (ForwardIt i = first; ++i != last;)
            if (!p(*i))
                *first++ = lw_std::move(*i);
    return first;
}

/*
    Minimum/maximum operations
*/

// NOTE: Arduino defines a macro for max, so we can only alias max_of to max in non arduino environments
#ifndef ARDUINO
template <typename... Args>
[[nodiscard]] constexpr auto max(Args... args) {
    return max_of(args...);
}
#endif

// max (1) https://en.cppreference.com/w/cpp/algorithm/max
template <typename T>
[[nodiscard]] constexpr const T& max_of(const T& a, const T& b) {
    return a < b ? b : a;
}

// max (2) https://en.cppreference.com/w/cpp/algorithm/max
template <typename T, typename Compare>
[[nodiscard]] constexpr const T& max_of(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}

// FIXME: max (3) https://en.cppreference.com/w/cpp/algorithm/max
// FIXME: max (4) https://en.cppreference.com/w/cpp/algorithm/max

// NOTE: Arduino defines a macro for min, so we can only alias min_of to min in non arduino environments
#ifndef ARDUINO
template <typename... Args>
[[nodiscard]] constexpr auto min(Args... args) {
    return min_of(args...);
}
#endif

// min (1) https://en.cppreference.com/w/cpp/algorithm/min
template <typename T>
[[nodiscard]] constexpr const T& min_of(const T& a, const T& b) {
    return a < b ? a : b;
}

// min (2) https://en.cppreference.com/w/cpp/algorithm/min
template <typename T, typename Compare>
[[nodiscard]] constexpr const T& min_of(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? a : b;
}

// FIXME: min (3) https://en.cppreference.com/w/cpp/algorithm/min
// FIXME: min (4) https://en.cppreference.com/w/cpp/algorithm/min

/*
    Comparison operations
*/

// equal (1) https://en.cppreference.com/w/cpp/algorithm/equal
template <typename InputIt1, typename InputIt2>
[[nodiscard]] constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
    return equal(
        first1, last1, first2,
        [](typename InputIt1::value_type x, typename InputIt2::value_type y) constexpr { return x == y; });
}

// FIXME: equal (2) https://en.cppreference.com/w/cpp/algorithm/equal

// equal (3) https://en.cppreference.com/w/cpp/algorithm/equal
template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
[[nodiscard]] constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!p(*first1, *first2))
            return false;
    }

    return true;
}

// FIXME: equal (4) https://en.cppreference.com/w/cpp/algorithm/equal

// equal (5) https://en.cppreference.com/w/cpp/algorithm/equal
template <typename InputIt1, typename InputIt2>
[[nodiscard]] constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
    return equal(
        first1, last1, first2, last2,
        [](const typename InputIt1::reference a, const typename InputIt2::reference b) constexpr { return a == b; });
}

// FIXME: equal (6) https://en.cppreference.com/w/cpp/algorithm/equal

// equal (7) https://en.cppreference.com/w/cpp/algorithm/equal
template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
[[nodiscard]] constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (!p(*first1, *first2))
            return false;
    }

    return true;
}

// FIXME: equal (8) https://en.cppreference.com/w/cpp/algorithm/equal

// lexicographical_compare (1) https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
template <typename InputIt1, typename InputIt2>
[[nodiscard]] constexpr bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
    return lexicographical_compare(
        first1, last1, first2, last2,
        [](const typename InputIt1::reference a, const typename InputIt1::reference b) constexpr { return a < b; });
}

// FIXME: lexicographical_compare (2) https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare

// lexicographical_compare (3) https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
template <typename InputIt1, typename InputIt2, typename Compare>
[[nodiscard]] constexpr bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        bool comp_1_2 = comp(*first1, *first2);
        bool comp_2_1 = comp(*first2, *first1);
        if (!comp_1_2 && !comp_2_1)
            return comp_1_2;
    }

    return first1 == last1 && first2 != last2;
}

// FIXME: lexicographical_compare (4) https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare

}  // namespace lw_std