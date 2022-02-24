#pragma once

#include "common_container_impl.hpp"
#include "unique_ptr.hpp"
#include "utility.hpp"

#ifdef ARDUINO
#    include "Arduino.h"
#else
#    include <cstring>
#endif

namespace lw_std {

// lw_std implementation of std::vector
template <typename T>
class vector {
   protected:
    LWSTD_COMMON_ITERATOR_TYPES(vector, T, unique_ptr<T>&);
    LWSTD_COMMON_MEMBER_TYPES(T);

   public:
    ~vector() {
        clear();
    }

    vector() = default;

    vector(vector<T>& other) {
        operator=(other);
    };

    vector(vector<T>&& other) {
        operator=(other);
    };

    vector<T>& operator=(const vector<T>& other) {
        for (size_t i = 0; i < m_allocated_size; ++i)
            delete m_data[i];
        if (m_allocated_size) delete[] m_data;

        m_size = other.m_size;
        m_allocated_size = other.m_allocated_size;

        m_data = new T*[m_allocated_size];
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = new T((*other.m_data)[i]);

        return *this;
    }

    vector<T>& operator=(vector<T>&& other) {
        if (&other != this) {
            swap(m_data, other.m_data);
            swap(m_size, other.m_size);
            swap(m_allocated_size, other.m_allocated_size);
        }
        return *this;
    }

    size_type size() const {
        return m_size;
    };

    bool empty() const {
        return m_size == 0;
    };

    iterator begin() {
        return iterator(m_data[0]);
    };

    iterator end() {
        return iterator(m_data[m_size]);
    };

    const_iterator begin() const {
        return const_iterator(m_data[0]);
    };

    const_iterator end() const {
        return const_iterator(m_data[m_size]);
    };

    const_iterator cbegin() const {
        return const_iterator(m_data[0]);
    };

    const_iterator cend() const {
        return const_iterator(m_data[m_size]);
    };

    reference front() {
        return *(m_data[0]);
    };

    reference back() {
        return *(m_data[m_size - 1]);
    };

    const_reference front() const {
        return *(m_data[0]);
    };

    const_reference back() const {
        return *(m_data[m_size - 1]);
    };

    reference operator[](size_type index) {
        return *(m_data[index]);
    };

    reference at(size_type index) {
        // NOTE: 'at' should throw if index is out of range, but lw_std works without exceptions
        //       this will only help to eliminate out of bounds access / segmentation faults if size > 0
        if (index > m_size - 1)
            return front();
        else
            return *(m_data[index]);
    };

    // FIXME: avoid code duplication
    const_reference at(size_type index) const {
        if (index > m_size - 1)
            return front();
        else
            return *(m_data[index]);
    };

    void clear() {
        if (m_allocated_size) {
            delete[] m_data;
            m_allocated_size = 0;
            m_size = 0;
            m_data = nullptr;
        }
    }

    void push_back(const_reference elt) {
        emplace_back(elt);
    };

    void pop_back() {
        if (m_size > 0) {
            m_size--;
        }
    }

    void insert(const_iterator& it, const_reference elt) {
        emplace(it, elt);
    }

    template <typename... Args>
    void emplace_back(Args&&... arguments) {
        emplace(end(), arguments...);
    }

    template <typename... Args>
    void emplace(const_iterator& it, Args&&... arguments) {
        // NOTE: iterator might get invalidated on grow, so store index now
        auto index = index_from_iterator(it);

        if (m_size == m_allocated_size)
            grow();

        m_size++;

        shift_right(index, 1);
        m_data[index] = new T{arguments...};
    }

    const_iterator find(const_reference elt) const {
        return find_helper(elt, match_function);
    }

    iterator erase(const_iterator& it) {
        shift_left(index_from_iterator(it), 1);
        m_size--;

        return it;
    }

   protected:
    class iterator_ {
       public:
        unique_ptr<T>* m_data{nullptr};

        iterator_() = default;

        iterator_(unique_ptr<T>& data)
            : m_data(&data) {}

        T& get() {
            return *m_data->get();
        }

        bool equals(const iterator_& other) const {
            return m_data == other.m_data;
        }

        void advance(int n) {
            m_data += n;
        }

        void copy_impl(const iterator_& other) {
            m_data = other.m_data;
        }

        void move_impl(iterator_& other) {
            m_data = move(other.m_data);
        }
    };

    unique_ptr<T>* m_data = nullptr;

    size_type m_size = 0;
    size_type m_allocated_size = 0;

    static constexpr bool match_function(const_reference a, const_reference b) {
        return a == b;
    }

    void shift_left(size_type from, size_type by) {
        for (size_type j = from + by; j < m_size; ++j)
            m_data[j - by] = move(m_data[j]);
    }

    void shift_right(ssize_t from, size_type by) {
        for (ssize_t j = m_size - 1 - by; j >= from; --j)
            m_data[j + by] = move(m_data[j]);
    }

    void grow() {
        size_type new_size = m_allocated_size + 8;
        auto new_data = new unique_ptr<T>[new_size];

        for (size_type i = 0; i < m_size; i++)
            new_data[i] = move(m_data[i]);

        if (m_allocated_size != 0)
            delete[] m_data;

        m_allocated_size = new_size;
        m_data = new_data;
    }

    template <typename Type, typename Predicate>
    const_iterator find_helper(const Type& elt, Predicate is_match) const {
        for (size_type i = 0; i < m_size; i++)
            if (is_match(*(m_data[i]), elt))
                return const_iterator(m_data[i]);
        return end();
    }

    size_type index_from_iterator(const_iterator& it) const {
        return it.m_data - m_data;
    }
};

}  // namespace lw_std