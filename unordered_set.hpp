#pragma once

#include "list.hpp"

namespace lw_std {

// lw_std implementation of std::unordered_set
template <typename T>
class unordered_set : private list<T> {
   private:
    using list<T>::push_back;
    using list<T>::pop_back;

   public:
    typedef typename list<T>::iterator iterator;
    using list<T>::find;
    using list<T>::end;
    using list<T>::begin;
    using list<T>::empty;
    using list<T>::size;
    using list<T>::erase;

    void insert(const T& elt) {
        auto res = find(elt);
        if (res == end())
            push_back(elt);
    }

    size_t erase(const T& key) {
        auto res = find(key);

        if (res == end()) {
            return 0;
        } else {
            erase(res);
            return 1;
        }
    }
};

}  // namespace lw_std