#pragma once

#include "algorithm.hpp"
#include "impl/iterator.hpp"
#include "impl/member_types.hpp"
#include "memory.hpp"
#include "unique_ptr.hpp"
#include "utility.hpp"

namespace lw_std {

// lw_std implementation of std::list
template <typename T, typename Allocator = allocator<T>>
class list {
   protected:
    struct list_item_t;
    struct list_item_t {
        unique_ptr<T> elt;
        list_item_t* next;
        list_item_t* prev;
    };

    LWSTD_COMMON_CONTAINER_TYPES(Allocator);
    LWSTD_COMMON_VALUE_TYPES(T);

   public:
    ~list() {
        clear();
    }

    list() = default;

    list(list<T>& other) {
        operator=(other);
    };

    list(list<T>&& other) {
        operator=(lw_std::move(other));
    };

    list<T>& operator=(const list<T>& other) {
        if (&other != this) {
            clear();

            for (auto& item : other) {
                push_back(item);
            }
        }

        return *this;
    }

    list<T>& operator=(list<T>&& other) {
        if (&other != this) {
            lw_std::swap(m_front, other.m_front);
            lw_std::swap(m_back, other.m_back);
            lw_std::swap(m_size, other.m_size);
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
        return iterator(m_front);
    };

    iterator end() {
        return iterator(nullptr);
    };

    const_iterator begin() const {
        return const_iterator(m_front);
    };

    const_iterator end() const {
        return const_iterator(nullptr);
    };

    const_iterator cbegin() const {
        return const_iterator(m_front);
    };

    const_iterator cend() const {
        return const_iterator(nullptr);
    };

    reference front() {
        return *(m_front->elt);
    };

    reference back() {
        return *(m_back->elt);
    };

    const_reference front() const {
        return *(m_front->elt);
    };

    const_reference back() const {
        return *(m_back->elt);
    };

    void clear() {
        while (m_front) {
            auto next = m_front->next;
            delete m_front;
            m_front = next;
        }
        m_back = nullptr;
        m_size = 0;
    }

    template <typename... Args>
    reference emplace_back(Args... arguments) {
        return *emplace(nullptr, arguments...);
    }

    void push_back(const_reference elt) {
        insert(nullptr, elt);
    }

    template <typename... Args>
    reference emplace_front(Args... arguments) {
        return *emplace(m_front, arguments...);
    }

    void push_front(const_reference elt) {
        insert(m_front, elt);
    }

    iterator insert(const_iterator& it, const_reference elt) {
        list_item_t* new_item = new list_item_t();
        new_item->elt = new T(elt);

        return insert_list_item(it, new_item);
    }

    template <typename... Args>
    iterator emplace(const iterator& it, Args... arguments) {
        list_item_t* new_item = new list_item_t();
        new_item->elt = new T{arguments...};

        return insert_list_item(it, new_item);
    }

    void pop_back() {
        if (m_back == nullptr)
            return;

        auto to_delete = m_back;
        m_back = m_back->prev;
        m_back->next = nullptr;

        delete to_delete;
        m_size--;
    }

    void pop_front() {
        if (m_front == nullptr)
            return;

        if (m_back == m_front)
            m_back = nullptr;

        auto to_delete = m_front;
        m_front = m_front->next;

        if (m_front != nullptr)
            m_front->prev = nullptr;

        delete to_delete;
        m_size--;
    }

    const_iterator find(const_reference elt) const {
        return lw_std::find(begin(), end(), elt);
    }

    iterator erase(const_iterator& it) {
        list_item_t* item = it.m_data;
        auto return_it = it + 1;

        if (item->prev)
            item->prev->next = item->next;
        else
            m_front = item->next;

        if (item->next)
            item->next->prev = item->prev;
        else
            m_back = item->prev;

        delete item;
        m_size--;

        return return_it;
    }

    class iterator : public iterator_impl<iterator, T> {
        friend iterator_impl<iterator, T>;
        friend list;

       public:
        iterator() = default;

        iterator(list_item_t* it)
            : m_data(it) {}

       private:
        list_item_t* m_data;
        T* get() {
            return m_data->elt.get();
        }

        const T* get() const {
            return m_data->elt.get();
        }

        bool equals(const iterator& rhs) const {
            return m_data == rhs.m_data;
        }

        void advance(int n) {
            for (int i = 0; i < n && m_data; ++i)
                m_data = m_data->next;

            for (int i = 0; i > n && m_data; --i)
                m_data = m_data->prev;
        }

        void copy(const iterator& other) {
            m_data = other.m_data;
        }

        void move(iterator& other) {
            m_data = lw_std::move(other.m_data);
        }
    };

    list_item_t* m_front = nullptr;
    list_item_t* m_back = nullptr;

    size_type m_size = 0;

    iterator insert_list_item(const_iterator& it, list_item_t* new_item) {
        // if we are inserting before end, we are the new back and have no next elt
        if (it.m_data == nullptr) {
            if (m_back) {
                new_item->prev = m_back;
                m_back->next = new_item;
            }

            m_back = new_item;
        } else {
            if (it.m_data->prev) {
                new_item->prev = it.m_data->prev;
                it.m_data->prev->next = new_item;
            }

            new_item->next = it.m_data;
            it.m_data->prev = new_item;
        }

        if (it.m_data == m_front || m_front == nullptr)
            m_front = new_item;

        m_size++;

        return new_item;
    }
};

}  // namespace lw_std