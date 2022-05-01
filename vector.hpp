#pragma once

#include "algorithm.hpp"
#include "common_container_impl.hpp"
#include "limits.hpp"
#include "unique_ptr.hpp"
#include "utility.hpp"

namespace lw_std {

// FIXME: add allocator support: vector (1) https://en.cppreference.com/w/cpp/container/vector
template <typename T>
class vector {
   protected:
    /*
        Member types
    */

    LWSTD_COMMON_ITERATOR_TYPES(vector, T, unique_ptr<T>&);
    LWSTD_COMMON_MEMBER_TYPES(T);

   public:
    /*
        Member functions
    */

    // (constructor) (1) https://en.cppreference.com/w/cpp/container/vector/vector
    vector() noexcept = default;

    // FIXME: (constructor) (2) https://en.cppreference.com/w/cpp/container/vector/vector
    // FIXME: (constructor) (3) https://en.cppreference.com/w/cpp/container/vector/vector
    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/vector/vector
    // FIXME: (constructor) (5) https://en.cppreference.com/w/cpp/container/vector/vector

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
        if (m_data) {
            delete[] m_data;
            m_data = nullptr;
            m_allocated_size = 0;
            m_size = 0;
        }
    }

    // operator= (1) https://en.cppreference.com/w/cpp/container/vector/operator%3D
    constexpr vector& operator=(const vector<T>& other) noexcept {
        for (size_type i = 0; i < m_allocated_size; ++i)
            delete m_data[i];
        if (m_allocated_size) delete[] m_data;

        m_size = other.m_size;
        m_allocated_size = other.m_allocated_size;

        m_data = new T*[m_allocated_size];
        for (size_t i = 0; i < m_size; i++)
            m_data[i] = new T((*other.m_data)[i]);

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
            *m_data[i] = value;
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
            return *(m_data[pos]);
    };

    // at https://en.cppreference.com/w/cpp/container/vector/at
    // FIXME: avoid code duplication
    [[nodiscard]] constexpr const_reference at(size_type pos) const noexcept {
        if (pos > m_size - 1)
            return front();
        else
            return *(m_data[pos]);
    };

    // operator[] https://en.cppreference.com/w/cpp/container/vector/operator_at
    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept {
        return *(m_data[pos]);
    };

    // operator[] https://en.cppreference.com/w/cpp/container/vector/operator_at
    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept {
        return *(m_data[pos]);
    };

    // front https://en.cppreference.com/w/cpp/container/vector/front
    [[nodiscard]] constexpr reference front() noexcept {
        return *(m_data[0]);
    };

    // front https://en.cppreference.com/w/cpp/container/vector/front
    [[nodiscard]] constexpr const_reference front() const noexcept {
        return *(m_data[0]);
    };

    // back https://en.cppreference.com/w/cpp/container/vector/back
    [[nodiscard]] constexpr reference back() noexcept {
        return *(m_data[m_size - 1]);
    };

    // back https://en.cppreference.com/w/cpp/container/vector/back
    [[nodiscard]] constexpr const_reference back() const noexcept {
        return *(m_data[m_size - 1]);
    };

    // FIXME: data https://en.cppreference.com/w/cpp/container/vector/data

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
            grow(new_cap - m_allocated_size);
    }

    // capacity https://en.cppreference.com/w/cpp/container/vector/capacity
    [[nodiscard]] constexpr size_type capacity() const noexcept {
        return m_allocated_size;
    }

    // shrink_to_fit https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit
    constexpr void shrink_to_fit() noexcept {
        shrink(m_size);
    }

    /*
        Modifiers
    */

    // clear https://en.cppreference.com/w/cpp/container/vector/clear
    constexpr void clear() noexcept {
        for (size_type i = 0; i < m_size; ++i)
            m_data[i].~unique_ptr();
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
    constexpr iterator insert(const_iterator& pos, size_type count, const_reference value) noexcept {
        iterator it{pos};
        for (size_type i = 0; i < count; ++i) {
            it = emplace(it, value);
        }
        return it;
    }

    // insert (4) https://en.cppreference.com/w/cpp/container/vector/insert
    template <typename InputIt>
    constexpr iterator insert(const_iterator& pos, InputIt first, InputIt last) noexcept {
        iterator it{pos};
        for (; first != last; ++first) {
            it = emplace(it, *first);
        }
        return it;
    }

    // FIXME: insert (5) https://en.cppreference.com/w/cpp/container/vector/insert

    // emplace https://en.cppreference.com/w/cpp/container/vector/emplace
    template <typename... Args>
    constexpr iterator emplace(const_iterator& pos, Args&&... args) noexcept {
        // NOTE: iterator might get invalidated on grow, so store index now
        auto index = index_from_iterator(pos);

        if (m_size == m_allocated_size)
            grow();

        m_size++;

        shift_right(index, 1);
        m_data[index] = new T{args...};

        return m_data[index];
    }

    // erase (1) https://en.cppreference.com/w/cpp/container/vector/erase
    constexpr iterator erase(const_iterator& pos) noexcept {
        shift_left(index_from_iterator(pos), 1);

        // NOTE: the last element was moved from and does not need to be destroyed
        --m_size;

        return pos;
    }

    // erase (2) https://en.cppreference.com/w/cpp/container/vector/erase
    constexpr iterator erase(const_iterator& first, const_iterator& last) noexcept {
        iterator it = first;

        // NOTE: as we shift left after every deletion, the start iterator pos never changes
        size_type idx = index_from_iterator(first);

        for (; it != last; ++it) {
            shift_left(idx, 1);

            // NOTE: no need to call delete, because after the shift the last element was moved from
            m_size--;
        }

        return first + 1;
    }

    // push_back (1) https://en.cppreference.com/w/cpp/container/vector/push_back
    constexpr void push_back(const_reference value) noexcept {
        emplace_back(value);
    };

    // push_back (2) https://en.cppreference.com/w/cpp/container/vector/push_back
    constexpr void push_back(T&& value) noexcept {
        emplace_back(value);
    };

    // emplace_back https://en.cppreference.com/w/cpp/container/vector/emplace_back
    template <typename... Args>
    constexpr reference emplace_back(Args&&... args) noexcept {
        return *emplace(end(), args...);
    }

    // pop_back https://en.cppreference.com/w/cpp/container/vector/pop_back
    constexpr void pop_back() noexcept {
        if (m_size > 0) {
            // NOTE: destroy popped element
            delete m_data[--m_size].release();
        }
    }

    // resize (1) https://en.cppreference.com/w/cpp/container/vector/resize
    constexpr void resize(size_type count) noexcept {
        // NOTE: additional default-inserted elements are appended
        resize(count, {});
    }

    // resize (2) https://en.cppreference.com/w/cpp/container/vector/resize
    constexpr void resize(size_type count, const_reference value) noexcept {
        if (count > m_allocated_size) {
            size_type diff = count < m_allocated_size;
            reserve(count);
            // NOTE: additional copies of value are appended
            for (size_type i = 0; i < diff; ++i)
                push_back(value);
        } else if (m_allocated_size > count)
            shrink(count);
    }

    // swap https://en.cppreference.com/w/cpp/container/vector/swap
    constexpr void swap(vector& other) noexcept {
        swap(*this, other);
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

    unique_ptr<T>* m_data{nullptr};

    size_type m_size{0};
    size_type m_allocated_size{0};

    void shift_left(size_type from, size_type by) noexcept {
        for (size_type j = from + by; j < m_size; ++j)
            m_data[j - by] = move(m_data[j]);
    }

    void shift_right(ssize_t from, size_type by) noexcept {
        for (ssize_t j = m_size - 1 - by; j >= from; --j)
            m_data[j + by] = move(m_data[j]);
    }

    void grow(size_type by = 8) noexcept {
        size_type new_size = m_allocated_size + by;
        auto new_data = new unique_ptr<T>[new_size];

        for (size_type i = 0; i < m_size; i++)
            new_data[i] = move(m_data[i]);

        if (m_allocated_size != 0)
            delete[] m_data;

        m_allocated_size = new_size;
        m_data = new_data;
    }

    void shrink(size_type to_size) noexcept {
        auto new_data = new unique_ptr<T>[to_size];

        for (size_type i = 0; i < to_size; i++)
            new_data[i] = move(m_data[i]);

        if (m_allocated_size != 0)
            delete[] m_data;

        m_allocated_size = to_size;
        m_data = new_data;
    }

    size_type index_from_iterator(const_iterator& it) const noexcept {
        return it.m_data - m_data;
    }
};

/*
    Non-member functions
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