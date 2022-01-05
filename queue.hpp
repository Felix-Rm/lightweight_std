#pragma once

#include "list.hpp"

namespace lw_std {

// lw_std implementation of std::queue
template <typename T>
class queue : private list<T> {
   private:
    using list<T>::push_back;
    using list<T>::pop_back;

   public:
    // push item to queue
    void push(const T& elt) { this->push_back(elt); };

    // pop item from queue
    void pop() { this->pop_front(); };

    using list<T>::size;
    using list<T>::empty;
    using list<T>::front;
    using list<T>::back;
};

}  // namespace lw_std