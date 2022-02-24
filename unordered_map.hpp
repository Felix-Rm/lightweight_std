#pragma once

#include "unordered_set.hpp"
#include "utility.hpp"

namespace lw_std {

// lw_std implementation of std::unordered_map
template <typename T, typename U>
class unordered_map : private unordered_set<pair<T, U>> {
   private:
    static constexpr bool match_function(const pair<T, U>& a, const T& b) {
        return a.first == b;
    }

    U& at_helper(const T& key) const {
        iterator res = this->find(key);
        if (res != this->end()) {
            return res->second;
        }

        // NOTE: lw_std doesn't throw on lookup failure
        return this->back().second;
    }

   public:
    typedef typename unordered_set<pair<T, U>>::iterator iterator;
    typedef typename unordered_set<pair<const T, U>>::const_iterator const_iterator;

    using typename unordered_set<pair<T, U>>::value_type;

    using unordered_set<pair<T, U>>::erase;
    using unordered_set<pair<T, U>>::begin;
    using unordered_set<pair<T, U>>::end;
    using unordered_set<pair<T, U>>::size;
    using unordered_set<pair<T, U>>::empty;

    // find element in map by key
    iterator find(const T& key) {
        return this->find_helper(key, match_function);
    }

    const_iterator find(const T& key) const {
        return this->find_helper(key, match_function);
    }

    size_t erase(const T& key) {
        auto res = find(key);
        if (res != this->end()) {
            erase(res);
            return 1;
        }

        return 0;
    }

    U& at(const T& key) {
        return at_helper(key);
    };

    U& at(const T& key) const {
        return at_helper(key);
    };

    //  element acces with operator[]
    U& operator[](const T& key) {
        auto res = find(key);
        if (res != this->end())
            return res->second;

        this->push_back({key, {}});
        return this->back().second;
    }

    template <typename... Args>
    void emplace(const T& key, Args... arguments) {
        auto res = find(key);
        if (res == this->end())
            unordered_set<pair<T, U>>::emplace(key, arguments...);
    }

    template <template <typename, typename> class S, typename A, typename B>
    void emplace(const S<A, B>& arg) {
        emplace(arg.first, arg.second);
    }

    template <template <typename, typename> class S, typename A, typename B>
    void insert(const S<A, B>& arg) {
        emplace(arg.first, arg.second);
    }
};

}  // namespace lw_std