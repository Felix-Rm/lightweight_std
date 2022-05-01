#pragma once

#include "impl/member_types.hpp"
#include "list.hpp"

namespace lw_std {
// FIXME: this file only includes the definitions for the queue container https://en.cppreference.com/w/cpp/container/queue,
//        not the header itself https://en.cppreference.com/w/cpp/header/queue

template <typename T, typename Container = list<T>>
class queue {
   public:
    /*
        MEMBER TYPES
    */

    LWSTD_COMMON_VALUE_TYPES(T);
    typedef Container container_type;

    /*
        MEMBER FUNCTIONS
    */

    // (constructor) (1) https://en.cppreference.com/w/cpp/container/queue/queue
    queue() = default;

    // (constructor) (2) https://en.cppreference.com/w/cpp/container/queue/queue
    explicit queue(const Container& cont)
        : m_container(cont){};

    // (constructor) (3) https://en.cppreference.com/w/cpp/container/queue/queue
    explicit queue(const Container&& cont)
        : m_container(lw_std::move(cont)){};

    // (constructor) (4) https://en.cppreference.com/w/cpp/container/queue/queue
    queue(const queue& other) {
        operator=(other);
    };

    // (constructor) (5) https://en.cppreference.com/w/cpp/container/queue/queue
    queue(queue&& other) {
        operator=(lw_std::move(other));
    };

    // FIXME: (constructor) (6) https://en.cppreference.com/w/cpp/container/queue/queue
    // FIXME: (constructor) (7) https://en.cppreference.com/w/cpp/container/queue/queue
    // FIXME: (constructor) (8) https://en.cppreference.com/w/cpp/container/queue/queue
    // FIXME: (constructor) (9) https://en.cppreference.com/w/cpp/container/queue/queue
    // FIXME: (constructor) (10) https://en.cppreference.com/w/cpp/container/queue/queue
    // FIXME: (constructor) (11) https://en.cppreference.com/w/cpp/container/queue/queue
    // FIXME: (constructor) (12) https://en.cppreference.com/w/cpp/container/queue/queue

    // (destructor) https://en.cppreference.com/w/cpp/container/queue/~queue
    ~queue() = default;

    // operator= (1) https://en.cppreference.com/w/cpp/container/queue/operator%3D
    constexpr queue& operator=(const queue& other) {
        m_container = other.m_container;
        return *this;
    }

    // operator= (2) https://en.cppreference.com/w/cpp/container/queue/operator%3D
    constexpr queue& operator=(queue&& other) {
        m_container = lw_std::move(other.m_container);
        return *this;
    }

    /*
        Element access
    */

    // front https://en.cppreference.com/w/cpp/container/queue/front
    [[nodiscard]] constexpr reference front() {
        return m_container.front();
    }

    // front https://en.cppreference.com/w/cpp/container/queue/front
    [[nodiscard]] constexpr const_reference front() const {
        return m_container.front();
    }

    // back https://en.cppreference.com/w/cpp/container/queue/back
    [[nodiscard]] constexpr reference back() {
        return m_container.back();
    }

    // back https://en.cppreference.com/w/cpp/container/queue/back
    [[nodiscard]] constexpr const_reference back() const {
        return m_container.back();
    }

    /*
        Capacity
    */

    // empty https://en.cppreference.com/w/cpp/container/queue/empty
    [[nodiscard]] constexpr bool empty() const {
        return m_container.empty();
    }

    // size https://en.cppreference.com/w/cpp/container/queue/size
    [[nodiscard]] constexpr size_type size() const {
        return m_container.size();
    }

    /*
        Modifiers
    */

    // push (1) https://en.cppreference.com/w/cpp/container/queue/push
    constexpr void push(const value_type& value) {
        m_container.emplace_back(value);
    };

    // push (2) https://en.cppreference.com/w/cpp/container/queue/push
    constexpr void push(value_type&& value) {
        m_container.emplace_back(lw_std::move(value));
    };

    constexpr void pop() {
        m_container.pop_front();
    };

    template <typename... Args>
    constexpr reference emplace(Args... args) {
        return m_container.emplace_back(args...);
    }

   private:
    Container m_container{};
};

}  // namespace lw_std