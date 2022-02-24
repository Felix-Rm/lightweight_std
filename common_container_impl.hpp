#pragma once

#include "utility.hpp"

namespace lw_std {

template <typename container, typename iterator, typename... Args>
class iterator_impl : private iterator {
    friend container;
    friend iterator;

   protected:
    typedef typename container::value_type T;

    iterator_impl& copy(const iterator_impl& other) {
        this->copy_impl(other);
        return *this;
    };

    iterator_impl& move(iterator_impl& other) {
        if (&other != this)
            this->move_impl(other);
        return *this;
    };

   public:
    iterator_impl() = default;

    iterator_impl(Args... args)
        : iterator(args...) {}

    iterator_impl(const iterator_impl& other) {
        this->copy(other);
    }

    iterator_impl(iterator_impl&& other) {
        this->move(other);
    }

    iterator_impl& operator=(const iterator_impl& other) {
        this->copy(other);
    }

    iterator_impl& operator=(iterator_impl&& other) {
        this->move(other);
    }

    iterator_impl& operator++() {
        this->advance(1);
        return *this;
    }

    iterator_impl operator++(int) {
        iterator_impl tmp(*this);
        this->advance(1);
        return tmp;
    }

    iterator_impl operator+(int n) const {
        iterator_impl tmp(*this);
        tmp.advance(n);
        return tmp;
    }

    bool operator==(const iterator_impl& rhs) const {
        return this->equals(rhs);
    }

    bool operator!=(const iterator_impl& rhs) const {
        return !operator==(rhs);
    }

    T& operator*() {
        return this->get();
    }

    T& operator->() {
        return this->get();
    }

    const T& operator*() const {
        return this->get();
    };

    const T* operator->() const {
        return this->get();
    };
};

#define LWSTD_COMMON_ITERATOR_TYPES(C, T, ...)                             \
    class iterator_;                                                       \
                                                                           \
   public:                                                                 \
    typedef iterator_impl<C, iterator_, __VA_ARGS__> iterator;             \
    typedef const iterator_impl<C, iterator_, __VA_ARGS__> const_iterator; \
    using difference_type = ptrdiff_t;

#define LWSTD_COMMON_MEMBER_TYPES(T) \
   public:                           \
    using value_type = T;            \
    using size_type = size_t;        \
    using reference = value_type&;   \
    using const_reference = const value_type&;

}  // namespace lw_std