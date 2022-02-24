#pragma once

#include "common_container_impl.hpp"
#include "unique_ptr.hpp"
#include "utility.hpp"

namespace lw_std {

// lw_std implementation of std::list
template <typename T>
class list {
   protected:
    struct list_item;
    typedef struct list_item {
        unique_ptr<T> elt;
        list_item* next;
        list_item* prev;
    } list_item_t;

    LWSTD_COMMON_ITERATOR_TYPES(list, T, list_item*);
    LWSTD_COMMON_MEMBER_TYPES(T);

   public:
    ~list() {
        clear();
    }

    list() = default;

    list(list<T>& other) {
        operator=(other);
    };

    list(list<T>&& other) {
        operator=(other);
    };

    list<T>& operator=(const list<T>& other) {
        clear();

        for (auto& item : other) {
            push_back(item);
        }

        return *this;
    }

    list<T>& operator=(list<T>&& other) {
        if (&other != this) {
            swap(m_front, other.m_front);
            swap(m_back, other.m_back);
            swap(m_size, other.m_size);
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
        return emplace(nullptr, arguments...);
    }

    void push_back(const_reference elt) {
        insert(nullptr, elt);
    }

    template <typename... Args>
    reference emplace_front(Args... arguments) {
        return emplace(m_front, arguments...);
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
    reference emplace(const iterator& it, Args... arguments) {
        list_item_t* new_item = new list_item_t();
        new_item->elt = new T{arguments...};

        return *insert_list_item(it, new_item);
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
        return find_helper(elt, match_function);
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

   protected:
    class iterator_ {
       public:
        list_item_t* m_data;

        iterator_() = default;

        iterator_(list_item_t* it)
            : m_data(it) {}

        T& get() {
            return *(m_data->elt);
        }

        bool equals(const iterator_& rhs) const {
            return m_data == rhs.m_data;
        }

        void advance(int n) {
            for (int i = 0; i < n && m_data; ++i)
                m_data = m_data->next;

            for (int i = 0; i > n && m_data; --i)
                m_data = m_data->prev;
        }

        void copy_impl(const iterator_& other) {
            m_data = other.m_data;
        }

        void move_impl(iterator_& other) {
            m_data = move(other.m_data);
        }
    };

    list_item_t* m_front = nullptr;
    list_item_t* m_back = nullptr;

    size_type m_size = 0;

    static constexpr bool match_function(const_reference a, const_reference b) {
        return a == b;
    }

    template <typename Type, typename Predicate>
    const_iterator find_helper(const Type& elt, Predicate is_match) const {
        list_item_t* item = m_front;
        while (item) {
            if (is_match(*(item->elt), elt))
                return const_iterator(item);
            item = item->next;
        }
        return end();
    }

    iterator insert_list_item(const_iterator& it, list_item* new_item) {
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