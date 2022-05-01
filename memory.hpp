#pragma once

#include "impl/member_types.hpp"
#include "limits.hpp"
#include "utility.hpp"

namespace lw_std {

template <typename T>
class allocator {
   public:
    /*
        MEMBER TYPES
    */

    LWSTD_COMMON_VALUE_TYPES(T);
    LWSTD_COMMON_POINTER_TYPES(T);

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
    constexpr allocator(const allocator<U>&) noexcept {};

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
    [[nodiscard]] constexpr pointer allocate(size_type num) noexcept {
        return (pointer)(::operator new(num * sizeof(value_type)));
    }

    // deallocate https://en.cppreference.com/w/cpp/memory/allocator/deallocate
    constexpr void deallocate(pointer p, size_type num) noexcept {
        ::operator delete(p, num * sizeof(value_type));
    }

    // max_size https://en.cppreference.com/w/cpp/memory/allocator/max_size
    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // construct https://en.cppreference.com/w/cpp/memory/allocator/construct
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) noexcept {
        new ((void*)p) T(lw_std::forward<Args>(args)...);
    }

    // destroy https://en.cppreference.com/w/cpp/memory/allocator/destroy
    template <typename U>
    void destroy(U* p) {
        p->~T();
    }
};

/*
    NON-MEMBER FUNCTIONS
*/

// operator== (1) https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
template <class T1, class T2>
bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
    return true;
}

// operator!= (2) https://en.cppreference.com/w/cpp/memory/allocator/operator_cmp
template <class T1, class T2>
bool operator!=(const allocator<T1>&, const allocator<T2>&) noexcept {
    return false;
};

}  // namespace lw_std