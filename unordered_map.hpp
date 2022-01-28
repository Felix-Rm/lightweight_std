#pragma once

#include "list.hpp"
#include "utility.hpp"

namespace lw_std {

// lw_std implementation of std::unordered_map
template <typename T, typename U>
class unordered_map : private list<pair<T, U>> {
   private:
    using list<pair<T, U>>::push_back;
    using list<pair<T, U>>::pop_back;
    using list<pair<T, U>>::back;
    using list<pair<T, U>>::front;

    static constexpr bool match_function(const pair<T, U>& a, const T& b) {
        return a.first == b;
    }

   public:
    typedef typename list<pair<T, U>>::iterator iterator;
    using list<pair<T, U>>::end;
    using list<pair<T, U>>::begin;
    using list<pair<T, U>>::empty;
    using list<pair<T, U>>::size;

    // find element in map by element
    using list<pair<T, U>>::erase;

    // find element in map by key
    iterator find(const T& key) const {
        return this->find_helper(key, match_function);
    }

    // erase element from map by key
    size_t erase(const T& key) {
        auto res = find(key);

        if (res == end()) {
            return 0;
        } else {
            erase(res);
            return 1;
        }
    }

    // insert element into map
    void insert(const pair<T, U>& elt) {
        auto res = find(elt.first);
        if (res == end())
            push_back(elt);
    }

    //  element acces with operator[]
    U& operator[](const T& key) {
        auto res = find(key);
        if (res != end())
            return res->second;

        push_back({key, {}});
        return back().second;
    }
};

}  // namespace lw_std