#pragma once

#include "impl/hash_container.hpp"

namespace lw_std {
// unordered_map header https://en.cppreference.com/w/cpp/container/unordered_map

template <typename T, typename U, typename Hash = hash<T>, typename Equal = equal_to<T>, typename Allocator = allocator<pair<T, U>>>
class unordered_map : public hash_container_impl<pair<T, U>, T, Hash, Equal, Allocator> {
   private:
    using underlying_type = hash_container_impl<pair<T, U>, T, Hash, Equal>;

   public:
    /*
        MEMBER TYPES
    */

    using mapped_type = U;

    /*
        MEMBER FUNCTIONS
    */

    /*
        Lookup
    */

    // at (1) https://en.cppreference.com/w/cpp/container/unordered_map/at
    mapped_type& at(const typename underlying_type::key_type& key) {
        return (mapped_type&)this->find_hash(key, this->hash_element(key))->second;
    }

    // at (2) https://en.cppreference.com/w/cpp/container/unordered_map/at
    const mapped_type& at(const typename underlying_type::key_type& key) const {
        return this->find_hash(key, this->hash_element(key))->second;
    }

    // operator[] (1) https://en.cppreference.com/w/cpp/container/unordered_map/at
    mapped_type& operator[](const typename underlying_type::key_type& key) {
        auto& res = this->find_hash(key, this->hash_element(key));
        if (res == this->end()) return this->emplace(lw_std::move(typename underlying_type::value_type{key, {}})).first->second;
        else return (mapped_type&)res->second;
    }

    // operator[] (2) https://en.cppreference.com/w/cpp/container/unordered_map/at
    mapped_type& operator[](typename underlying_type::key_type&& key) {
        auto& res = this->find_hash(key, this->hash_element(key));
        if (res == this->end()) return this->emplace(lw_std::move(typename underlying_type::value_type{lw_std::move(key), {}})).first->second;
        else return (mapped_type&)res->second;
    }

   private:
    const typename underlying_type::key_type& key_access_proxy(typename underlying_type::const_reference elt) const override {
        return elt.first;
    }
};

}  // namespace lw_std