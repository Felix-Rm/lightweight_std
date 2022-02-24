#pragma once

#include "list.hpp"

namespace lw_std {

// lw_std implementation of std::queue
template <typename T, typename ContainerType = list<T>>
class queue {
    LWSTD_COMMON_MEMBER_TYPES(T);

   public:
    typedef ContainerType container_type;

    constexpr size_type size() const {
        return m_container.size();
    }

    constexpr bool empty() const {
        return m_container.empty();
    }

    constexpr reference front() {
        return m_container.front();
    }

    constexpr const_reference front() const {
        return m_container.front();
    }

    constexpr reference back() {
        return m_container.back();
    }

    constexpr const_reference back() const {
        return m_container.back();
    }

    constexpr void push(const T& elt) {
        m_container.push_back(elt);
    };

    constexpr void pop() {
        m_container.pop_front();
    };

    template <typename... Args>
    constexpr reference emplace(Args... args) {
        return m_container.emplace_back(args...);
    }

   private:
    ContainerType m_container{};
};

}  // namespace lw_std