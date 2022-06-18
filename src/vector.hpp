#pragma once

#include "algorithm.hpp"
#include "impl/iterator.hpp"
#include "impl/member_types.hpp"
#include "limits.hpp"
#include "memory.hpp"
#include "unique_ptr.hpp"
#include "utility.hpp"

namespace lw_std {
// FIXME: this file only includes the definitions for the vector container https://en.cppreference.com/w/cpp/container/vector,
//        not the header itself https://en.cppreference.com/w/cpp/header/vector

template <typename T, typename Allocator = allocator<T>>
class vector {
   public:
    /*
       MEMBER TYPES
   */

    LWSTD_COMMON_VALUE_TYPES(T);
    LWSTD_COMMON_POINTER_TYPES(T);
    LWSTD_COMMON_CONTAINER_TYPES(Allocator);

    /*
        MEMBER FUNCTIONS
    */

    // (constructor) (1) https://en.cppreference.com/w/cpp/container/vector/vector
    vector() noexcept = default;

    // FIXME: (constructor) (2) https://en.cppreference.com/w/cpp/container/vector/vector

    // FIXME: (constructor) (3) https://en.cppreference.com/w/cpp/container/vector/vector
    constexpr vector(size_type count, const_reference value) noexcept {
        resize(count, value);
    }

    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/vector/vector
    constexpr vector(size_type count) noexcept {
        resize(count);
    }

    // FIXME: (constructor) (5) https://en.cppreference.com/w/cpp/container/vector/vector
    template <class InputIt>
    constexpr vector(InputIt first, InputIt last) {
        insert(begin(), first, last);
    }

    // (constructor) (6) https://en.cppreference.com/w/cpp/container/vector/vector
    constexpr vector(const vector<T>& other) noexcept {
        operator=(other);
    };

    // FIXME: (constructor) (7) https://en.cppreference.com/w/cpp/container/vector/vector

    // (constructor) (8) https://en.cppreference.com/w/cpp/container/vector/vector
    constexpr vector(vector<T>&& other) noexcept {
        operator=(lw_std::move(other));
    };

    // FIXME: (constructor) (9) https://en.cppreference.com/w/cpp/container/vector/vector

    // FIXME: (constructor) (10) https://en.cppreference.com/w/cpp/container/vector/vector

    // (destructor) https://en.cppreference.com/w/cpp/container/vector/~vector
    ~vector() noexcept {
        resize_impl(0);
    }

    // operator= (1) https://en.cppreference.com/w/cpp/container/vector/operator%3D
    constexpr vector& operator=(const vector<T>& other) noexcept {
        if (&other != this) {
            reserve(other.size());

            for (size_type i = 0; i < other.size() && i < m_size; ++i)
                (*this)[i] = other[i];

            for (size_type i = m_size; i < other.size(); ++i)
                m_allocator.construct(&m_data[i], other[i]);

            m_size = other.size();
        }

        return *this;
    }

    // operator= (2) https://en.cppreference.com/w/cpp/container/vector/operator%3D
    constexpr vector& operator=(vector<T>&& other) noexcept {
        if (&other != this) {
            lw_std::swap(m_data, other.m_data);
            lw_std::swap(m_size, other.m_size);
            lw_std::swap(m_allocated_size, other.m_allocated_size);
        }
        return *this;
    }

    // FIXME: operator= (3) https://en.cppreference.com/w/cpp/container/vector/operator%3D

    // assign (1) https://en.cppreference.com/w/cpp/container/vector/assign
    constexpr void assign(size_type count, const_reference value) noexcept {
        resize(count);
        for (size_type i = 0; i < count; ++i)
            m_data[i] = value;
    }

    // assign (2) https://en.cppreference.com/w/cpp/container/vector/assign
    // FIXME: this can probably be done better (allocate once?)
    template <typename InputIt>
    constexpr void assign(InputIt first, InputIt last) noexcept {
        clear();
        for (; first != last; ++first)
            push_back(*first);
    }

    // FIXME: assign (3) https://en.cppreference.com/w/cpp/container/vector/assign

    // FIXME: get_allocator https://en.cppreference.com/w/cpp/container/vector/get_allocator

    /*
        Element access
    */

    // at https://en.cppreference.com/w/cpp/container/vector/at
    [[nodiscard]] constexpr reference at(size_type pos) noexcept {
        // NOTE: 'at' should throw if index is out of range, but lw_std works without exceptions
        //       this will only help to eliminate out of bounds access / segmentation faults if size > 0
        if (pos > m_size - 1)
            return front();
        else
            return m_data[pos];
    };

    // at https://en.cppreference.com/w/cpp/container/vector/at
    // FIXME: avoid code duplication
    [[nodiscard]] constexpr const_reference at(size_type pos) const noexcept {
        if (pos > m_size - 1)
            return front();
        else
            return m_data[pos];
    };

    // operator[] https://en.cppreference.com/w/cpp/container/vector/operator_at
    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept {
        return m_data[pos];
    };

    // operator[] https://en.cppreference.com/w/cpp/container/vector/operator_at
    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept {
        return m_data[pos];
    };

    // front https://en.cppreference.com/w/cpp/container/vector/front
    [[nodiscard]] constexpr reference front() noexcept {
        return m_data[0];
    };

    // front https://en.cppreference.com/w/cpp/container/vector/front
    [[nodiscard]] constexpr const_reference front() const noexcept {
        return m_data[0];
    };

    // back https://en.cppreference.com/w/cpp/container/vector/back
    [[nodiscard]] constexpr reference back() noexcept {
        return m_data[m_size - 1];
    };

    // back https://en.cppreference.com/w/cpp/container/vector/back
    [[nodiscard]] constexpr const_reference back() const noexcept {
        return m_data[m_size - 1];
    };

    // data https://en.cppreference.com/w/cpp/container/vector/data
    [[nodiscard]] constexpr pointer data() noexcept {
        return m_data;
    }

    // data https://en.cppreference.com/w/cpp/container/vector/data
    [[nodiscard]] constexpr const_pointer data() const noexcept {
        return m_data;
    }

    /*
        Iterators
    */

    // begin https://en.cppreference.com/w/cpp/container/vector/begin
    [[nodiscard]] constexpr iterator begin() noexcept {
        return m_data[0];
    };

    // begin https://en.cppreference.com/w/cpp/container/vector/begin
    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return m_data[0];
    };

    // cbegin https://en.cppreference.com/w/cpp/container/vector/begin
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return m_data[0];
    };

    // end https://en.cppreference.com/w/cpp/container/vector/end
    [[nodiscard]] constexpr iterator end() noexcept {
        return m_data[m_size];
    };

    // end https://en.cppreference.com/w/cpp/container/vector/end
    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return m_data[m_size];
    };

    // cend https://en.cppreference.com/w/cpp/container/vector/end
    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return m_data[m_size];
    };

    // FIXME: rbegin https://en.cppreference.com/w/cpp/container/vector/rbegin
    // FIXME: rend https://en.cppreference.com/w/cpp/container/vector/rend

    /*
        Capacity
    */

    // empty https://en.cppreference.com/w/cpp/container/vector/empty
    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_size == 0;
    };

    // size https://en.cppreference.com/w/cpp/container/vector/size
    [[nodiscard]] constexpr size_type size() const noexcept {
        return m_size;
    };

    // max_size https://en.cppreference.com/w/cpp/container/vector/max_size
    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return numeric_limits<size_type>::max();
    }

    // reserve https://en.cppreference.com/w/cpp/container/vector/reserve
    constexpr void reserve(size_type new_cap) noexcept {
        if (new_cap > m_allocated_size)
            resize_impl(new_cap);
    }

    // capacity https://en.cppreference.com/w/cpp/container/vector/capacity
    [[nodiscard]] constexpr size_type capacity() const noexcept {
        return m_allocated_size;
    }

    // shrink_to_fit https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit
    constexpr void shrink_to_fit() noexcept {
        resize_impl(m_size);
    }

    /*
        Modifiers
    */

    // clear https://en.cppreference.com/w/cpp/container/vector/clear
    constexpr void clear() noexcept {
        for (size_type i = 0; i < m_size; ++i)
            m_allocator.destroy(&m_data[i]);
        m_size = 0;
    }

    // insert (1) https://en.cppreference.com/w/cpp/container/vector/insert
    constexpr iterator insert(const_iterator& pos, const_reference value) noexcept {
        return emplace(pos, value);
    }

    // insert (2) https://en.cppreference.com/w/cpp/container/vector/insert
    constexpr iterator insert(const_iterator& pos, T&& value) noexcept {
        return emplace(pos, lw_std::move(value));
    }

    // insert (3) https://en.cppreference.com/w/cpp/container/vector/insert
    constexpr iterator insert(iterator pos, size_type count, const_reference value) noexcept {
        auto index = index_from_iterator(pos);

        for (size_type i = 0; i < count; ++i)
            pos = emplace(pos, value);

        return m_data[index];
    }

    // insert (4) https://en.cppreference.com/w/cpp/container/vector/insert
    template <typename InputIt>
    constexpr iterator insert(const_iterator& pos, InputIt first, InputIt last) noexcept {
        auto index = index_from_iterator(pos);
        auto start_index = index;

        while (first != last) {
            emplace(m_data[index++], *first);
            first++;
        }

        return m_data[start_index];
    }

    // FIXME: insert (5) https://en.cppreference.com/w/cpp/container/vector/insert

    // emplace https://en.cppreference.com/w/cpp/container/vector/emplace
    template <typename... Args>
    constexpr iterator emplace(const_iterator& pos, Args&&... args) noexcept {
        // NOTE: iterator might get invalidated on grow, so store index now
        auto index = index_from_iterator(pos);

        if (m_size == m_allocated_size)
            resize_impl(m_size + 8);

        if (m_size == index) {
            m_allocator.construct(&m_data[m_size], lw_std::forward<Args>(args)...);
        } else {
            m_allocator.construct(&m_data[m_size], lw_std::move(m_data[m_size - 1]));
            shift(m_size - 1, index, -1);
            m_data[index] = lw_std::move(T(lw_std::forward<Args>(args)...));
        }

        m_size++;
        return m_data[index];
    }

    // erase (1) https://en.cppreference.com/w/cpp/container/vector/erase
    constexpr iterator erase(const_iterator& pos) noexcept {
        auto index = index_from_iterator(pos);

        shift(index, m_size - 1, 1);
        m_allocator.destroy(&m_data[--m_size]);

        return pos;
    }

    // erase (2) https://en.cppreference.com/w/cpp/container/vector/erase
    constexpr iterator erase(const_iterator& first, const_iterator& last) noexcept {
        size_type start_idx = index_from_iterator(first);
        size_type end_idx = index_from_iterator(last);
        size_type num_deleted = end_idx - start_idx;

        if (num_deleted > 0) {
            shift(start_idx, m_size - num_deleted, num_deleted);

            for (size_t i = 0; i < num_deleted; ++i)
                m_allocator.destroy(&m_data[--m_size]);
        }

        return first;
    }

    // push_back (1) https://en.cppreference.com/w/cpp/container/vector/push_back
    constexpr void push_back(const_reference value) noexcept {
        emplace_back(value);
    };

    // push_back (2) https://en.cppreference.com/w/cpp/container/vector/push_back
    constexpr void push_back(T&& value) noexcept {
        emplace_back(lw_std::move(value));
    };

    // emplace_back https://en.cppreference.com/w/cpp/container/vector/emplace_back
    template <typename... Args>
    constexpr reference emplace_back(Args&&... args) noexcept {
        return *emplace(end(), lw_std::forward<Args>(args)...);
    }

    // pop_back https://en.cppreference.com/w/cpp/container/vector/pop_back
    constexpr void pop_back() noexcept {
        if (m_size > 0) {
            m_allocator.destroy(&m_data[--m_size]);
        }
    }

    // resize (1) https://en.cppreference.com/w/cpp/container/vector/resize
    constexpr void resize(size_type count) noexcept {
        resize_impl(count, T());
    }

    // resize (2) https://en.cppreference.com/w/cpp/container/vector/resize
    constexpr void resize(size_type count, const_reference value) noexcept {
        resize_impl(count, value);
    }

    // swap https://en.cppreference.com/w/cpp/container/vector/swap
    constexpr void swap(vector& other) noexcept {
        lw_std::swap(*this, other);
    }

    class iterator : public iterator_impl<iterator, T> {
        friend iterator_impl<iterator, T>;
        friend vector;

       public:
        iterator() = default;

        iterator(reference data)
            : m_data(&data) {}

       private:
        T* m_data{nullptr};

        T* get() {
            return m_data;
        }

        const T* get() const {
            return m_data;
        }

        bool equals(const iterator& other) const {
            return m_data == other.m_data;
        }

        void advance(int n) {
            m_data += n;
        }

        void copy(const iterator& other) {
            m_data = other.m_data;
        }

        void move(iterator& other) {
            m_data = lw_std::move(other.m_data);
        }
    };

    pointer m_data{nullptr};
    allocator_type m_allocator{};

    size_type m_size{0};
    size_type m_allocated_size{0};

    void shift(size_type from, size_type to, int by) noexcept {
        auto dir = by > 0 ? 1 : -1;
        while (from != to) {
            m_data[from] = lw_std::move(m_data[from + by]);
            from += dir;
        }
    }

    void resize_impl(size_type new_size) noexcept {
        auto new_data = new_size > 0 ? m_allocator.allocate(new_size) : nullptr;

        for (size_type i = 0; i < m_size && i < new_size; ++i)
            m_allocator.construct(&new_data[i], lw_std::move(m_data[i]));

        for (size_type i = new_size; i < m_size; ++i)
            m_allocator.destroy(&m_data[i]);

        if (m_allocated_size != 0)
            m_allocator.deallocate(m_data, m_allocated_size);

        m_size = min_of(m_size, new_size);
        m_allocated_size = new_size;
        m_data = new_data;
    }

    void resize_impl(size_type count, const_reference value) noexcept {
        resize_impl(count);

        while (m_size < count) {
            emplace_back(value);
        }
    }

    size_type index_from_iterator(const_iterator& it) const noexcept {
        return it.m_data - m_data;
    }
};

/*
    NON-MEMBER FUNCTIONS
*/

// operator== (1) https://en.cppreference.com/w/cpp/container/vector/operator_cmp
template <typename T>
constexpr bool operator==(const vector<T>& lhs, const vector<T>& rhs) noexcept {
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
}

// operator== (2) https://en.cppreference.com/w/cpp/container/vector/operator_cmp
template <typename T>
constexpr bool operator!=(const vector<T>& lhs, const vector<T>& rhs) noexcept {
    return !operator==(lhs, rhs);
}

// operator< (3) https://en.cppreference.com/w/cpp/container/vector/operator_cmp
template <typename T>
constexpr bool operator<(const vector<T>& lhs, const vector<T>& rhs) noexcept {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), synthesized_cmp_less<T, T>);
}

// operator<= (4) https://en.cppreference.com/w/cpp/container/vector/operator_cmp
template <typename T>
constexpr bool operator<=(const vector<T>& lhs, const vector<T>& rhs) noexcept {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), synthesized_cmp_less_equal<T, T>);
}

// operator> (5) https://en.cppreference.com/w/cpp/container/vector/operator_cmp
template <typename T>
constexpr bool operator>(const vector<T>& lhs, const vector<T>& rhs) noexcept {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), synthesized_cmp_greater<T, T>);
}

// operator>= (6) https://en.cppreference.com/w/cpp/container/vector/operator_cmp
template <typename T>
constexpr bool operator>=(const vector<T>& lhs, const vector<T>& rhs) noexcept {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), synthesized_cmp_greater_equal<T, T>);
}

// FIXME: operator<=> (7) https://en.cppreference.com/w/cpp/container/vector/operator_cmp

// NOTE: no extra specialization: swap https://en.cppreference.com/w/cpp/container/vector/swap2
// NOTE: no extra specialization: erase (1) https://en.cppreference.com/w/cpp/container/vector/erase2
// NOTE: no extra specialization: erase (2) https://en.cppreference.com/w/cpp/container/vector/erase2

}  // namespace lw_std