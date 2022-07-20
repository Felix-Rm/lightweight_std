// list header https://en.cppreference.com/w/cpp/header/list
#pragma once

#include "algorithm.hpp"
#include "impl/iterator.hpp"
#include "impl/member_types.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace lw_std {

// list https://en.cppreference.com/w/cpp/container/list
template <typename T, typename Allocator = allocator<T>>
class list {
   protected:
    struct node {
        template <typename... U>
        constexpr node(node* prev_node, node* next_node, U&&... v)
            : value(lw_std::forward<U>(v)...), prev(prev_node), next(next_node) {
            if (prev)
                prev->next = this;
            if (next)
                next->prev = this;
        }

        ~node() {
            if (prev)
                prev->next = next;
            if (next)
                next->prev = prev;
        }

        T value;
        node* prev{nullptr};
        node* next{nullptr};
    };

    typedef typename Allocator::template rebind<node>::other node_allocator_t;

   public:
    /*
        MEMBER TYPES
    */

    LWSTD_COMMON_VALUE_TYPES(T)
    LWSTD_COMMON_CONTAINER_TYPES(T, node*, Allocator)

    /*
        MEMBER FUNCTIONS
    */

    // (constructor) (1) https://en.cppreference.com/w/cpp/container/list/list
    constexpr list() = default;

    // FIXME: (constructor) (2) https://en.cppreference.com/w/cpp/container/list/list

    // FIXME: (constructor) (3) https://en.cppreference.com/w/cpp/container/list/list
    constexpr list(list<T>& other) {
        operator=(other);
    }

    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/list/list
    constexpr list(list<T>&& other) {
        operator=(lw_std::move(other));
    }

    // FIXME: (constructor) (5) https://en.cppreference.com/w/cpp/container/list/list

    // FIXME: (destructor)  https://en.cppreference.com/w/cpp/container/list/~list
    ~list() {
        clear();
    }

    // operator= (1) https://en.cppreference.com/w/cpp/container/list/operator%3D
    constexpr list<T>& operator=(const list<T>& other) {
        if (&other != this) {
            clear();

            for (auto& item : other) {
                push_back(item);
            }
        }

        return *this;
    }

    // operator= (2) https://en.cppreference.com/w/cpp/container/list/operator%3D
    constexpr list<T>& operator=(list<T>&& other) {
        if (&other != this) {
            lw_std::swap(m_front, other.m_front);
            lw_std::swap(m_back, other.m_back);
            lw_std::swap(m_size, other.m_size);
        }
        return *this;
    }

    // FIXME: operator= (3) https://en.cppreference.com/w/cpp/container/list/operator%3D

    // assign (1) https://en.cppreference.com/w/cpp/container/list/assign
    constexpr void assign(size_type count, const_reference value) {
        clear();
        for (size_type i = 0; i < count; ++i)
            push_back(value);
    }

    // assign (2) https://en.cppreference.com/w/cpp/container/list/assign
    template <typename InputIt>
    constexpr void assign(InputIt first, InputIt last) {
        clear();
        for (; first != last; ++first)
            push_back(*first);
    }

    // FIXME: assign (3) https://en.cppreference.com/w/cpp/container/list/assign

    // get_allocator https://en.cppreference.com/w/cpp/container/list/get_allocator
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return m_allocator;
    }

    /*
        Element access
    */

    // front https://en.cppreference.com/w/cpp/container/list/front
    [[nodiscard]] constexpr reference front() {
        return m_front->value;
    }

    // front https://en.cppreference.com/w/cpp/container/list/front
    [[nodiscard]] constexpr const_reference front() const {
        return m_front->value;
    }

    // back https://en.cppreference.com/w/cpp/container/list/back
    [[nodiscard]] constexpr reference back() {
        return m_back->value;
    }

    // back https://en.cppreference.com/w/cpp/container/list/back
    [[nodiscard]] constexpr const_reference back() const {
        return m_back->value;
    }

    /*
        Iterators
    */

    // begin https://en.cppreference.com/w/cpp/container/list/begin
    [[nodiscard]] constexpr iterator begin() noexcept {
        return iterator(m_front);
    }

    // begin https://en.cppreference.com/w/cpp/container/list/begin
    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return const_iterator(m_front);
    }

    // begin https://en.cppreference.com/w/cpp/container/list/begin
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return const_iterator(m_front);
    }

    // end https://en.cppreference.com/w/cpp/container/list/end
    [[nodiscard]] constexpr iterator end() noexcept {
        return iterator(static_cast<node*>(nullptr));
    }

    // end https://en.cppreference.com/w/cpp/container/list/end
    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return const_iterator(static_cast<node*>(nullptr));
    }

    // end https://en.cppreference.com/w/cpp/container/list/end
    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return const_iterator(static_cast<node*>(nullptr));
    }

    // FIXME: rbegin https://en.cppreference.com/w/cpp/container/list/rbegin
    // FIXME: rend https://en.cppreference.com/w/cpp/container/list/rend

    /*
        Capacity
    */

    // empty https://en.cppreference.com/w/cpp/container/list/empty
    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_size == 0;
    }

    // size https://en.cppreference.com/w/cpp/container/list/size
    [[nodiscard]] constexpr size_type size() const noexcept {
        return m_size;
    }

    // max_size https://en.cppreference.com/w/cpp/container/list/max_size
    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return numeric_limits<size_type>::max();
    }

    /*
        Modifiers
    */

    // clear https://en.cppreference.com/w/cpp/container/list/clear
    constexpr void clear() noexcept {
        erase(cbegin(), cend());
    }

    // insert (1) https://en.cppreference.com/w/cpp/container/list/insert
    iterator insert(const_iterator it, const_reference value) {
        return emplace(it, value);
    }

    // insert (2) https://en.cppreference.com/w/cpp/container/list/insert
    iterator insert(const_iterator it, T&& value) {
        return emplace(it, lw_std::move(value));
    }

    // insert (3) https://en.cppreference.com/w/cpp/container/list/insert
    constexpr iterator insert(const_iterator pos, size_type count, const_reference value) {
        iterator ret_it = to_non_const_iterator(pos);

        for (size_type i = 0; i < count; ++i) {
            auto emplaced_node = emplace(pos, value);
            if (i == 0) ret_it = emplaced_node;
        }

        return ret_it;
    }

    // insert (4) https://en.cppreference.com/w/cpp/container/list/insert
    template <typename InputIt>
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
        bool ret_set{false};
        iterator ret_it = to_non_const_iterator(pos);

        while (first != last) {
            pos = emplace(pos, *first);
            if (!ret_set) {
                ret_it = to_non_const_iterator(pos);
                ret_set = true;
            }
            first++;
            pos++;
        }

        return ret_it;
    }

    // FIXME: insert (5) https://en.cppreference.com/w/cpp/container/list/insert

    // emplace https://en.cppreference.com/w/cpp/container/list/emplace
    template <typename... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args) {
        node* new_node = m_allocator.allocate(1);

        if (pos == cbegin()) {
            m_allocator.construct(new_node, nullptr, m_front, lw_std::forward<Args>(args)...);
            m_front = new_node;
        } else if (pos == cend()) {
            m_allocator.construct(new_node, m_back, nullptr, lw_std::forward<Args>(args)...);
            m_back = new_node;
        } else {
            const auto& underlying_it = static_cast<typename const_iterator::underlying_type>(pos);
            auto* mutable_item = const_cast<node*>(underlying_it.m_data);
            m_allocator.construct(new_node, mutable_item->prev, mutable_item, lw_std::forward<Args>(args)...);
        }

        if (empty()) m_back = m_front = new_node;

        m_size++;
        return new_node;
    }

    // erase (1) https://en.cppreference.com/w/cpp/container/list/erase
    constexpr iterator erase(const_iterator pos) {
        auto* mutable_item = to_non_const_iterator(pos).m_data;
        auto ret_it = iterator(mutable_item->next);

        if (mutable_item->prev)
            mutable_item->prev->next = mutable_item->next;
        else
            m_front = mutable_item->next;

        if (mutable_item->next)
            mutable_item->next->prev = mutable_item->prev;
        else
            m_back = mutable_item->prev;

        m_allocator.destroy(mutable_item);
        m_allocator.deallocate(mutable_item, 1);
        m_size--;

        return ret_it;
    }

    // erase (2) https://en.cppreference.com/w/cpp/container/list/erase
    constexpr iterator erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }

        return to_non_const_iterator(first);
    }

    // push_back (1) https://en.cppreference.com/w/cpp/container/list/push_back
    constexpr void push_back(const_reference value) {
        emplace_back(value);
    }

    // push_back (2) https://en.cppreference.com/w/cpp/container/list/push_back
    constexpr void push_back(T&& value) {
        emplace_back(lw_std::move(value));
    }

    // emplace_back https://en.cppreference.com/w/cpp/container/list/emplace_back
    template <typename... Args>
    constexpr reference emplace_back(Args&&... args) {
        return *emplace(cend(), lw_std::forward<Args>(args)...);
    }

    // pop_back https://en.cppreference.com/w/cpp/container/list/pop_back
    constexpr void pop_back() {
        if (!empty()) {
            auto tmp = m_back;
            m_back = m_back->prev;
            erase(tmp);
        }
    }

    // push_front (1) https://en.cppreference.com/w/cpp/container/list/push_front
    constexpr void push_front(const_reference value) {
        emplace_front(value);
    }

    // push_front (2) https://en.cppreference.com/w/cpp/container/list/push_front
    constexpr void push_front(T&& value) {
        emplace_front(lw_std::move(value));
    }

    // emplace_front https://en.cppreference.com/w/cpp/container/list/emplace_front
    template <typename... Args>
    constexpr reference emplace_front(Args&&... args) {
        return *emplace(cbegin(), lw_std::forward<Args>(args)...);
    }

    // pop_front https://en.cppreference.com/w/cpp/container/list/pop_front
    constexpr void pop_front() {
        if (!empty()) {
            auto tmp = m_front;
            m_front = m_front->next;
            erase(tmp);
        }
    }

    // FIXME: resize (1) https://en.cppreference.com/w/cpp/container/list/resize
    // FIXME: resize (2) https://en.cppreference.com/w/cpp/container/list/resize

    // FIXME: swap https://en.cppreference.com/w/cpp/container/list/swap

    /*
        Operations
    */

    // FIXME: merge (1) https://en.cppreference.com/w/cpp/container/list/merge
    // FIXME: merge (2) https://en.cppreference.com/w/cpp/container/list/merge

    // FIXME: splice (1) https://en.cppreference.com/w/cpp/container/list/splice
    // FIXME: splice (2) https://en.cppreference.com/w/cpp/container/list/splice
    // FIXME: splice (3) https://en.cppreference.com/w/cpp/container/list/splice

    // FIXME: remove (1) https://en.cppreference.com/w/cpp/container/list/remove
    // FIXME: remove_if (2) https://en.cppreference.com/w/cpp/container/list/remove

    // FIXME: reverse https://en.cppreference.com/w/cpp/container/list/reverse

    // FIXME: unique (1) https://en.cppreference.com/w/cpp/container/list/unique
    // FIXME: unique (2) https://en.cppreference.com/w/cpp/container/list/unique

    // FIXME: sort (1) https://en.cppreference.com/w/cpp/container/list/sort
    // FIXME: sort (2) https://en.cppreference.com/w/cpp/container/list/sort

   private:
    template <typename P, typename IT_P>
    class iterator_def {
        friend list;

       protected:
        typedef P value_type;
        typedef IT_P data_type;

        constexpr iterator_def() = default;

        constexpr iterator_def(const IT_P& data)
            : m_data(data) {}

        template <typename Q, typename IT_Q>
        constexpr iterator_def(const iterator_def<Q, IT_Q>& other)
            : m_data(other.m_data) {}

        constexpr iterator_def(const iterator_def& other) {
            operator=(other);
        }

        constexpr iterator_def(iterator_def&& other) {
            operator=(lw_std::move(other));
        }

        constexpr iterator_def& operator=(const iterator_def& other) {
            m_data = other.m_data;
            return *this;
        }

        constexpr iterator_def& operator=(iterator_def&& other) {
            m_data = lw_std::move(other.m_data);
            return *this;
        }

        [[nodiscard]] constexpr bool equal(const iterator_def& other) const {
            return m_data == other.m_data;
        }

        [[nodiscard]] constexpr P* get() {
            return &(m_data->value);
        }

        [[nodiscard]] constexpr const P* get() const {
            return &(m_data->value);
        }

        constexpr void advance(int n) {
            for (int i = 0; i < n && m_data; ++i)
                m_data = m_data->next;

            for (int i = 0; i > n && m_data; --i)
                m_data = m_data->prev;
        }

       private:
        IT_P m_data;
    };

    node_allocator_t m_allocator;

    node* m_front = nullptr;
    node* m_back = nullptr;

    size_type m_size = 0;

    static iterator to_non_const_iterator(const_iterator& pos) {
        const auto& underlying_pos = static_cast<typename const_iterator::underlying_type>(pos);
        return const_cast<node*>(underlying_pos.m_data);
    }
};

}  // namespace lw_std