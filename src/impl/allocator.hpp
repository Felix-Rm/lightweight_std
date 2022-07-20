#pragma once

#include "../limits.hpp"
#include "../utility.hpp"
#include "member_types.hpp"

namespace lw_std {

// allocator https://en.cppreference.com/w/cpp/memory/allocator
template <typename T>
class allocator {
   public:
    /*
        MEMBER TYPES
    */

    LWSTD_COMMON_VALUE_TYPES(T)
    LWSTD_COMMON_POINTER_TYPES(T)

    // rebind allocator to type U
    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };

    /*
        MEMBER FUNCTIONS
    */

    // (constructor) (1) https://en.cppreference.com/w/cpp/memory/allocator/allocator
    constexpr allocator() noexcept = default;

    // (constructor) (2) https://en.cppreference.com/w/cpp/memory/allocator/allocator
    constexpr allocator(const allocator&) noexcept = default;

    // (constructor) (3) https://en.cppreference.com/w/cpp/memory/allocator/allocator
    template <typename U>
    constexpr allocator(const allocator<U>&) noexcept {}

    // (destructor) (3) https://en.cppreference.com/w/cpp/memory/allocator/~allocator
    ~allocator() = default;

    // address (1) https://en.cppreference.com/w/cpp/memory/allocator/address
    [[nodiscard]] constexpr pointer address(reference value) const noexcept {
        return &value;
    }

    // address (2) https://en.cppreference.com/w/cpp/memory/allocator/address
    [[nodiscard]] constexpr const_pointer address(const_reference value) const noexcept {
        return &value;
    }

    // allocate https://en.cppreference.com/w/cpp/memory/allocator/allocate
    [[nodiscard]] constexpr pointer allocate(size_type num) {
        return static_cast<pointer>(::operator new(num * sizeof(value_type)));
    }

    // deallocate https://en.cppreference.com/w/cpp/memory/allocator/deallocate
    constexpr void deallocate(pointer p, [[maybe_unused]] size_type num) {
        ::operator delete(p);//, num * sizeof(value_type)
    }

    // max_size https://en.cppreference.com/w/cpp/memory/allocator/max_size
    [[nodiscard]] constexpr size_type max_size() const {
        return numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // construct https://en.cppreference.com/w/cpp/memory/allocator/construct
    template <typename U, typename... Args>
    constexpr void construct(U* p, Args&&... args) {
        new (static_cast<void*>(p)) T(lw_std::forward<Args>(args)...);
    }

    // destroy https://en.cppreference.com/w/cpp/memory/allocator/destroy
    template <typename U>
    constexpr void destroy(U* p) {
        p->~T();
    }
};

/*
    NON-MEMBER FUNCTIONS
*/

// operator== (1) https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
template <class T1, class T2>
constexpr bool operator==(const allocator<T1>&, const allocator<T2>&) {
    return true;
}

// operator!= (2) https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
template <class T1, class T2>
constexpr bool operator!=(const allocator<T1>&, const allocator<T2>&) {
    return false;
}

}  // namespace lw_std