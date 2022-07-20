// unordered_map header https://en.cppreference.com/w/cpp/header/unordered_map
#pragma once

#include "impl/hash_container.hpp"

namespace lw_std {

// unordered_map https://en.cppreference.com/w/cpp/container/unordered_map
template <typename T, typename U, typename Hash = hash<T>, typename Equal = equal_to<T>, typename Allocator = allocator<pair<const T, U>>>
class unordered_map : public hash_container_impl<unordered_map<T, U, Hash, Equal, Allocator>, pair<const T, U>, T, Hash, Equal, Allocator> {
   private:
    using underlying_type = hash_container_impl<unordered_map<T, U, Hash, Equal, Allocator>, pair<const T, U>, T, Hash, Equal>;
    friend underlying_type;

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
    [[nodiscard]] constexpr mapped_type& at(const typename underlying_type::key_type& key) {
        return underlying_type::find_hash(*static_cast<underlying_type*>(this), key, this->hash_element(key)).elt.get()->second;
    }

    // at (2) https://en.cppreference.com/w/cpp/container/unordered_map/at
    [[nodiscard]] constexpr const mapped_type& at(const typename underlying_type::key_type& key) const {
        return underlying_type::find_hash(*static_cast<const underlying_type*>(this), key, this->hash_element(key)).elt.get()->second;
    }

    // operator[] (1) https://en.cppreference.com/w/cpp/container/unordered_map/operator_at
    constexpr mapped_type& operator[](const typename underlying_type::key_type& key) {
        typename underlying_type::iterator res = &underlying_type::find_hash(*static_cast<underlying_type*>(this), key, this->hash_element(key));
        if (res == this->end()) return this->emplace(lw_std::move(typename underlying_type::value_type{key, {}})).first->second;
        else return res->second;
    }

    // operator[] (2) https://en.cppreference.com/w/cpp/container/unordered_map/operator_at
    constexpr mapped_type& operator[](typename underlying_type::key_type&& key) {
        typename underlying_type::iterator res = &underlying_type::find_hash(*static_cast<underlying_type*>(this), key, this->hash_element(key));
        if (res == this->end()) return this->emplace(lw_std::move(typename underlying_type::value_type{lw_std::move(key), {}})).first->second;
        else return res->second;
    }

   protected:
    static constexpr const typename underlying_type::key_type& key_access_proxy(typename underlying_type::const_reference elt) {
        return elt.first;
    }
};

}  // namespace lw_std