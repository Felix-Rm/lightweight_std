#pragma once

#include "move.hpp"

namespace lw_std {

// FIXME: unique_ptr https://en.cppreference.com/w/cpp/memory/unique_ptr
template <typename T>
class unique_ptr {
   private:
    T* m_ptr{nullptr};

   public:
    /*
        MEMBER TYPES
    */

    using pointer = T*;
    using element_type = T;

    /*
        MEMBER FUNCTIONS
    */

    // FIXME: (constructor) (1) https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
    constexpr unique_ptr() noexcept = default;

    // (constructor) (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
    constexpr explicit unique_ptr(pointer p) noexcept
        : m_ptr(p) {}

    // FIXME: (constructor) (3) https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr

    // (constructor) (5) https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
    constexpr unique_ptr(unique_ptr&& other) noexcept {
        operator=(lw_std::move(other));
    }

    // FIXME: (constructor) (6) https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
    template <typename U>
    constexpr unique_ptr(unique_ptr<U>&& u) noexcept
        : m_ptr(u.m_ptr) {}

    // (destructor) https://en.cppreference.com/w/cpp/memory/unique_ptr/~unique_ptr
    ~unique_ptr() {
        if (m_ptr) {
            delete m_ptr;
            m_ptr = nullptr;
        };
    }

    // operator= (1) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D
    constexpr unique_ptr& operator=(unique_ptr&& r) noexcept {
        if (this != &r) {
            reset(r.release());
        }
        return *this;
    }

    // FIXME: operator= (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D
    template <typename U>
    constexpr unique_ptr& operator=(unique_ptr<U>&& r) noexcept {
        return operator=(r);
    }

    // FIXME: operator= (3) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D

    /*
        Modifiers
    */

    // release https://en.cppreference.com/w/cpp/memory/unique_ptr/release
    [[nodiscard]] constexpr pointer release() noexcept {
        auto tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    // reset (1) https://en.cppreference.com/w/cpp/memory/unique_ptr/reset
    constexpr void reset(pointer ptr = pointer()) noexcept {
        if (m_ptr) delete m_ptr;
        m_ptr = ptr;
    }

    // reset (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/reset
    template <typename U>
    constexpr void reset(U ptr) noexcept {
        operator=(ptr);
    }

    // FIXME: reset (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/reset

    /*
        Observers
    */

    // get https://en.cppreference.com/w/cpp/memory/unique_ptr/get
    [[nodiscard]] constexpr pointer get() const noexcept {
        return m_ptr;
    }

    // FIXME: get_deleter https://en.cppreference.com/w/cpp/memory/unique_ptr/get_deleter

    // operator bool https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_bool
    [[nodiscard]] constexpr operator bool() const noexcept {
        return m_ptr != nullptr;
    }

    // FIXME: operator* (1) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator*
    [[nodiscard]] constexpr T& operator*() const noexcept {
        return *m_ptr;
    }

    // operator-> (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator*
    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return m_ptr;
    }
};

/*
    NON-MEMBER FUNCTIONS
*/

// make_unique (1) https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
template <typename T, typename... Args>
[[nodiscard]] constexpr unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>{new T{lw_std::forward<Args>(args)...}};
}

// FIXME: make_unique (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
// FIXME: make_unique (3) https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique

// make_unique_for_overwrite (3) https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
template <typename T>
[[nodiscard]] constexpr unique_ptr<T> make_unique_for_overwrite() {
    return unique_ptr<T>{new T{}};
}

// FIXME: make_unique (5) https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
// FIXME: make_unique (6) https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique

// operator== (1) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
template <typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const unique_ptr<T1>& x, const unique_ptr<T1>& y) {
    return x.get() == y.get();
}

// operator!= (2) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
template <typename T1, typename T2>
[[nodiscard]] constexpr bool operator!=(const unique_ptr<T1>& x, const unique_ptr<T1>& y) {
    return !operator==(x, y);
}

// FIXME: operator< (3) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
// FIXME: operator<= (4) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
// FIXME: operator> (5) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cm
// FIXME: operator> (6) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
// FIXME: operator<=> (7) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
// FIXME: operator (8-20) https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp

}  // namespace lw_std
