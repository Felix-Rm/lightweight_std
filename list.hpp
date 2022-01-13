#pragma once

#include "utility.hpp"

namespace lw_std {

// lw_std implementation of std::vector
template <typename T>
class list {
   protected:
    struct list_item;
    typedef struct list_item {
        T elt;
        list_item *next;
        list_item *prev;
    } list_item_t;

    static list_item_t *successor(list_item_t *item, size_t n) {
        for (size_t i = 0; i < n && item->next; ++i) item = item->next;
        return item;
    }

   public:
    class iterator {
        friend list;

       private:
        list_item_t *p;

       public:
        iterator(list_item_t *x) : p(x) {}
        iterator(const list_item_t &mit) : p(mit.p) {}

        iterator &operator++() {
            p = p->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            operator++();
            return tmp;
        }

        iterator operator+(int n) const {
            return iterator{successor(p, n)};
        }

        bool operator==(const T *&rhs) const { return p->elt == rhs; }

        bool operator==(const iterator &rhs) const { return p == rhs.p; }
        bool operator!=(const iterator &rhs) const { return p != rhs.p; }

        T &operator*() { return p->elt; }
        T *operator->() { return &(p->elt); }

        const T &operator*() const { return p->elt; };
        const T *operator->() const { return &(p->elt); };
    };

   protected:
    list_item_t *m_front = nullptr;
    list_item_t *m_back = nullptr;

    size_t m_size = 0;

    static constexpr bool match_function(const T &a, const T &b) {
        return a == b;
    }

    template <typename Arg>
    iterator find_helper(const Arg &elt, bool (*is_match)(const T &, const Arg &)) const {
        list_item_t *item = m_front;
        while (item) {
            if (is_match(item->elt, elt))
                return iterator(item);
            item = item->next;
        }
        return end();
    }

    iterator erase_helper(const iterator &it) {
        list_item_t *item = it.p;
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

    void copy_from(const list<T> &other) {
        clear();

        for (auto &item : other) {
            push_back(item);
        }
    }

    void move_from(list<T> &&other) {
        swap(m_front, other.m_front);
        swap(m_back, other.m_back);
        swap(m_size, other.m_size);
    }

   public:
    ~list() {
        clear();
    }

    list() = default;

    list(list<T> &other) {
        copy_from(other);
    };

    list(list<T> &&other) {
        move_from(other);
    };

    list<T> &operator=(const list<T> &other) {
        copy_from(other);
        return *this;
    }

    list<T> &operator=(list<T> &&other) {
        move_from(move(other));
        return *this;
    }

    size_t size() const { return m_size; };
    bool empty() const { return m_size == 0; };

    iterator begin() const { return iterator(m_front); };
    iterator end() const { return iterator(nullptr); };

    T &front() const { return m_front->elt; };
    T &back() const { return m_back->elt; };

    void clear() {
        while (m_front) {
            auto next = m_front->next;
            delete m_front;
            m_front = next;
        }
        m_back = nullptr;
        m_size = 0;
    }

    void push_back(const T &elt) {
        insert(nullptr, elt);
    }

        if (m_front == nullptr) m_front = m_back;
    }

    void insert(const iterator &it, const T &elt) {
        list_item_t *new_item = new list_item_t();

        new_item->elt = elt;

        // if we are inserting before end, we are the new back and have no next elt
        if (it.p == nullptr) {
            if (m_back) {
                new_item->prev = m_back;
                m_back->next = new_item;
            }

            m_back = new_item;
        } else {
            if (it.p->prev) {
                new_item->prev = it.p->prev;
                it.p->prev->next = new_item;
            }

            new_item->next = it.p;
            it.p->prev = new_item;
        }

        if (it.p == m_front || m_front == nullptr)
            m_front = new_item;

        m_size++;
    }

    void pop_back() {
        if (m_back == nullptr) return;

        auto to_delete = m_back;
        m_back = m_back->prev;
        m_back->next = nullptr;

        delete to_delete;
        m_size--;
    }

    void pop_front() {
        if (m_front == nullptr) return;

        if (m_back == m_front) m_back = nullptr;

        auto to_delete = m_front;
        m_front = m_front->next;

        if (m_front != nullptr)
            m_front->prev = nullptr;

        delete to_delete;
        m_size--;
    }

    iterator find(const T &elt) const { return find_helper(elt, match_function); }

    iterator erase(const iterator &it) { return erase_helper(it); }
};

}  // namespace lw_std