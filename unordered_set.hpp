#pragma once

#include "impl/hash_container.hpp"

namespace lw_std {
// unordered_set header https://en.cppreference.com/w/cpp/container/unordered_set

template <typename T, typename Hash = hash<T>, typename Equal = equal_to<T>, typename Allocator = allocator<T>>
class unordered_set : public hash_container_impl<T, T, Hash, Equal, Allocator> {
   private:
    using underlying_type = hash_container_impl<T, T, Hash, Equal>;

    const typename underlying_type::key_type& key_access_proxy(typename underlying_type::const_reference elt) const override {
        return elt;
    }
};

}  // namespace lw_std