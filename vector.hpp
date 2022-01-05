#pragma once

#include "utility.hpp"

#ifdef ARDUINO
#include "Arduino.hpp"
#else
#include <cstring>
#endif

namespace lw_std {

// lw_std implementation of std::vector
template <typename T>
class vector {
   public:
    class iterator {
       private:
        T *p;

       public:
        iterator(T *x) : p(x) {}
        iterator(const iterator &mit) : p(mit.p) {}

        iterator &operator++() {
            ++p;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            operator++();
            return tmp;
        }

        iterator operator+(int n) const {
            iterator tmp(*this);
            tmp.p += n;
            return tmp;
        }

        bool operator==(const T *&rhs) const { return p == rhs; }

        bool operator==(const iterator &rhs) const { return p == rhs.p; }
        bool operator!=(const iterator &rhs) const { return p != rhs.p; }

        T &operator*() { return *p; }
        T *operator->() { return p; }

        const T &operator*() const { return *p; };
        const T *operator->() const { return p; };
    };

   protected:
    T *m_data = nullptr;

    size_t m_cursor = 0;
    size_t m_size = 0;

    static constexpr bool match_function(const T &a, const T &b) {
        return a == b;
    }

    void shift_left(size_t from, size_t by) {
        for (size_t j = from + by; j < m_cursor; j++)
            m_data[j - by] = move(m_data[j]);
    }

    void grow() {
        size_t new_size = m_size + 8;
        T *new_data = new T[new_size];

        for (size_t i = 0; i < m_cursor; i++)
            new_data[i] = (T &&) m_data[i];

        if (m_size != 0)
            delete[] m_data;

        m_size = new_size;
        m_data = new_data;
    }

    template <typename Arg>
    iterator find_helper(const Arg &elt, bool (*is_match)(const T &, const Arg &)) const {
        for (size_t i = 0; i < m_cursor; i++)
            if (is_match(m_data[i], elt))
                return iterator(&m_data[i]);
        return end();
    }

    iterator erase_helper(const iterator &it) {
        size_t index = &(*it) - m_data;
        shift_left(index, 1);
        m_cursor--;

        return it;
    }

    void copy_from(const vector<T> &other) {
        if (m_size)
            delete[] m_data;

        m_cursor = other.m_cursor;
        m_size = other.m_size;

        m_data = new T[m_size];
        for (size_t i = 0; i < m_cursor; i++)
            m_data[i] = (T &&) other.m_data[i];
    }

    void move_from(vector<T> &&other) {
        swap(m_data, other.m_data);
        swap(m_cursor, other.m_cursor);
        swap(m_size, other.m_size);
    }

   public:
    ~vector() {
        clear();
    }

    vector() = default;

    vector(vector<T> &other) {
        copy_from(other);
    };

    vector(vector<T> &&other) {
        move_from(other);
    };

    vector<T> &operator=(const vector<T> &other) {
        copy_from(other);
        return *this;
    }

    vector<T> &operator=(vector<T> &&other) {
        move_from(other);
        return *this;
    }

    size_t size() const { return m_cursor; };
    bool empty() const { return m_cursor == 0; };

    iterator begin() const { return iterator(&m_data[0]); };
    iterator end() const { return iterator(&m_data[m_cursor]); };

    T &front() const { return m_data[0]; };
    T &back() const { return m_data[m_cursor - 1]; };

    T &operator[](size_t index) { return m_data[index]; };

    void clear() {
        if (m_size)
            delete[] m_data;
    }

    void push_back(const T &elt) {
        if (m_cursor == m_size)
            grow();
        m_data[m_cursor++] = elt;
    };

    void pop_back() {
        if (m_cursor > 0) {
            m_cursor--;
        }
    }

    void pop_front() {
        if (m_cursor > 0) {
            shift_left(0, 1);
            m_cursor--;
        }
    }

    iterator find(const T &elt) const { return find_helper(elt, match_function); }

    iterator erase(const iterator &it) { return erase_helper(it); }
};

}  // namespace lw_std