#pragma once

#include "../functional.hpp"
#include "../vector.hpp"
#include "iterator.hpp"
#include "member_types.hpp"

namespace lw_std {

template <typename T, typename KeyType, typename Hash = hash<KeyType>, typename Equal = equal_to<KeyType>, typename Allocator = allocator<T>>
class hash_container_impl {
   protected:
    /*
        MEMBER TYPES
    */
    enum bucket_state {
        CLEAN,
        DELETED,
        REHASHED,
        END
    };

    typedef struct bucket {
        unique_ptr<T> elt{};
        bucket_state state{CLEAN};

        bucket() = default;

        bucket(const bucket& other) {
            operator=(other);
        }

        bucket(bucket&& other) {
            operator=(lw_std::move(other));
        }

        constexpr bucket& operator=(const bucket& other) {
            if (other.elt)
                elt = new T{*other.elt.get()};
            state = other.state;
            return *this;
        }

        constexpr bucket& operator=(bucket&& other) {
            elt = lw_std::move(other.elt);
            state = other.state;
            return *this;
        }
    } bucket_t;

    LWSTD_COMMON_CONTAINER_TYPES(Allocator);
    LWSTD_COMMON_VALUE_TYPES(T);

    using key_type = KeyType;
    using key_equal = Equal;
    using hasher = Hash;

   public:
    /*
        MEMBER FUNCTIONS
    */

    // FIXME: (constructor) (1) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set
    hash_container_impl() = default;

    // FIXME: (constructor) (2) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // (constructor) (3) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set
    hash_container_impl(const hash_container_impl& other) {
        operator=(other);
    }

    // FIXME: (constructor) (3) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set
    hash_container_impl(hash_container_impl&& other) {
        operator=(lw_std::move(other));
    }

    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // FIXME: (constructor) (5) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // (destructor) https://en.cppreference.com/w/cpp/container/unordered_set/~unordered_set
    ~hash_container_impl() = default;

    // operator= (1) https://en.cppreference.com/w/cpp/container/unordered_map/operator%3D
    constexpr hash_container_impl& operator=(const hash_container_impl& other) noexcept {
        m_size = other.m_size;
        m_buckets = other.m_buckets;
        return *this;
    }

    // operator= (2) https://en.cppreference.com/w/cpp/container/unordered_map/operator%3D
    constexpr hash_container_impl& operator=(hash_container_impl&& other) noexcept {
        m_size = other.m_size;
        m_buckets = lw_std::move(other.m_buckets);
        return *this;
    }

    // FIXME: operator= (3) https://en.cppreference.com/w/cpp/container/unordered_map/operator%3D

    // FIXME: get_allocator https://en.cppreference.com/w/cpp/container/unordered_set/get_allocator

    /*
        Iterators
    */

    // begin https://en.cppreference.com/w/cpp/container/unordered_set/begin
    [[nodiscard]] constexpr iterator begin() noexcept {
        return find_first_used_bucket();
    };

    // begin https://en.cppreference.com/w/cpp/container/unordered_set/begin
    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return find_first_used_bucket();
    };

    // cbegin https://en.cppreference.com/w/cpp/container/unordered_set/begin
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return find_first_used_bucket();
    };

    // end https://en.cppreference.com/w/cpp/container/unordered_set/end
    [[nodiscard]] constexpr iterator end() noexcept {
        return m_buckets.back();
    };

    // end https://en.cppreference.com/w/cpp/container/unordered_set/end
    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return m_buckets.back();
    };

    // cend https://en.cppreference.com/w/cpp/container/unordered_set/end
    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return m_buckets.back();
    };

    /*
        Capacity
    */

    // empty https://en.cppreference.com/w/cpp/container/unordered_set/empty
    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_size == 0;
    }

    // size https://en.cppreference.com/w/cpp/container/unordered_set/size
    [[nodiscard]] constexpr size_type size() const noexcept {
        return m_size;
    }

    // max_size https://en.cppreference.com/w/cpp/container/unordered_set/max_size
    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return numeric_limits<size_type>::max();
    }

    [[nodiscard]] constexpr size_type capacity() const noexcept {
        return m_buckets.capacity() - 1;  // end bucket not included
    }

    /*
        Modifiers
    */

    // clear https://en.cppreference.com/w/cpp/container/unordered_set/clear
    constexpr void clear() noexcept {
        m_buckets.resize(0);
        m_size = 0;
    }

    // insert (1) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    constexpr pair<iterator, bool> insert(const_reference value) noexcept {
        return emplace(lw_std::move(value));
    }

    // insert (2) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    constexpr pair<iterator, bool> insert(T&& value) noexcept {
        return emplace(lw_std::move(value));
    }

    // FIXME: insert (3) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    // FIXME: insert (4) https://en.cppreference.com/w/cpp/container/unordered_set/insert

    // insert (5) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    template <typename InputIt>
    constexpr void insert(InputIt first, InputIt last) noexcept {
        for (; first != last; ++first) {
            emplace(*first);
        }
    }

    // FIXME: insert (6) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    // FIXME: insert (7) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    // FIXME: insert (8) https://en.cppreference.com/w/cpp/container/unordered_set/insert

    // emplace https://en.cppreference.com/w/cpp/container/unordered_set/emplace
    template <class... Args>
    constexpr pair<iterator, bool> emplace(Args&&... args) noexcept {
        // NOTE: this will make sure the underlying vector has capacity > 0;
        //       if capacity is zero we would get division by zero in hash
        rehash_if_needed();

        // FIXME: use allocator
        unique_ptr<T> element = new T(lw_std::forward<Args>(args)...);
        auto hash = hash_element(key_access_proxy(*element));
        iterator res = find_hash(key_access_proxy(*element), hash);

        if (res == end()) {
            m_size++;
            return {insert_into_next_free_after(hash, element.release()), true};
        }

        return {res, false};
    }

    // FIXME: emplace_hint https://en.cppreference.com/w/cpp/container/unordered_set/emplace_hint

    // erase (1) https://en.cppreference.com/w/cpp/container/unordered_set/erase
    constexpr iterator erase(const_iterator pos) noexcept {
        pos.m_data->elt.destroy();
        pos.m_data->state = DELETED;
        m_size--;
        return pos + 1;
    }

    // erase (2) https://en.cppreference.com/w/cpp/container/unordered_set/erase
    constexpr iterator erase(const_iterator first, const_iterator last) noexcept {
        iterator pos;
        for (iterator it = first; it != last; it++)
            pos = erase(it);
        return pos;
    }

    // erase (3) https://en.cppreference.com/w/cpp/container/unordered_set/erase
    constexpr size_type erase(key_type key) noexcept {
        const_iterator res = find_hash(key, hash_element(key));
        if (res == end()) return 0;
        erase(res);
        return 1;
    }

    // FIXME: erase (4) https://en.cppreference.com/w/cpp/container/unordered_set/erase

    // FIXME: swap https://en.cppreference.com/w/cpp/container/unordered_set/swap

    // FIXME: extract https://en.cppreference.com/w/cpp/container/unordered_set/extract

    // FIXME: merge https://en.cppreference.com/w/cpp/container/unordered_set/merge

    /*
        Lookup
    */

    // count (1) https://en.cppreference.com/w/cpp/container/unordered_set/count
    [[nodiscard]] constexpr size_type count(const key_type& key) const noexcept {
        return find(key) == end() ? 0 : 1;
    }

    // FIXME: count (2) https://en.cppreference.com/w/cpp/container/unordered_set/count

    // find (1) https://en.cppreference.com/w/cpp/container/unordered_set/find
    [[nodiscard]] constexpr iterator find(const key_type& key) noexcept {
        return find_hash(key, hash_element(key));
    }

    // find (2) https://en.cppreference.com/w/cpp/container/unordered_set/find
    [[nodiscard]] constexpr const_iterator find(const key_type& key) const noexcept {
        return find_hash(key, hash_element(key));
    }

    // FIXME: find (3) https://en.cppreference.com/w/cpp/container/unordered_set/find
    // FIXME: find (4) https://en.cppreference.com/w/cpp/container/unordered_set/find

    // contains (1) https://en.cppreference.com/w/cpp/container/unordered_set/contains
    [[nodiscard]] constexpr bool contains(const key_type& key) const noexcept {
        return find(key) == end() ? false : true;
    }

    // FIXME: contains (2) https://en.cppreference.com/w/cpp/container/unordered_set/contains

    // FIXME: equal_range https://en.cppreference.com/w/cpp/container/unordered_set/equal_range

    /*
        Bucket interface
    */

    // FIXME: begin https://en.cppreference.com/w/cpp/container/unordered_set/begin2

    // FIXME: end https://en.cppreference.com/w/cpp/container/unordered_set/end2

    // bucket_count https://en.cppreference.com/w/cpp/container/unordered_set/bucket_count
    [[nodiscard]] constexpr size_type bucket_count() const noexcept {
        return m_buckets.capacity();
    }

    // max_bucket_count https://en.cppreference.com/w/cpp/container/unordered_set/max_bucket_count
    [[nodiscard]] constexpr size_type max_bucket_count() const noexcept {
        return m_buckets.max_size();
    }

    // bucket_size https://en.cppreference.com/w/cpp/container/unordered_set/bucket_size
    [[nodiscard]] constexpr size_type bucket_size(size_type n) const noexcept {
        return m_buckets[n].elt ? 1 : 0;
    }

    // bucket_size https://en.cppreference.com/w/cpp/container/unordered_set/bucket_size
    [[nodiscard]] constexpr size_type bucket(const key_type& key) const noexcept {
        return hash_element(key);
    }

    /*
        Hash policy
    */

    // FIXME: load_factor https://en.cppreference.com/w/cpp/container/unordered_set/load_factor

    // FIXME: max_load_factor https://en.cppreference.com/w/cpp/container/unordered_set/max_load_factor

    // rehash https://en.cppreference.com/w/cpp/container/unordered_set/rehash
    void rehash(size_type count) {
        rehash_if_needed(count);
    }

    // reserve https://en.cppreference.com/w/cpp/container/unordered_set/reserve
    void reserve(size_type count) {
        rehash_if_needed(count);
    }

    /*
        Observers
    */

    // FIXME: hash_function https://en.cppreference.com/w/cpp/container/unordered_set/hash_function

    // FIXME: key_eq https://en.cppreference.com/w/cpp/container/unordered_set/key_eq

    class iterator : public iterator_impl<iterator, T> {
        friend iterator_impl<iterator, T>;
        friend hash_container_impl;

       public:
        iterator() = default;

        iterator(bucket_t& data)
            : m_data(&data) {}

        iterator(const bucket_t& data)
            : m_data((bucket_t*)&data) {}

       private:
        bucket_t* m_data{nullptr};

        T* get() {
            return m_data->elt.get();
        }

        const T* get() const {
            return m_data->elt.get();
        }

        bool equals(const iterator& other) const {
            return m_data == other.m_data;
        }

        void advance(size_t n) {
            for (size_t i = 0; i < n; ++i)
                do {
                    if (m_data->state == END) return;
                    m_data++;
                } while (!m_data->elt);
        }

        void copy(const iterator& other) {
            m_data = other.m_data;
        }

        void move(iterator& other) {
            m_data = lw_std::move(other.m_data);
        }
    };

   protected:
    virtual const key_type& key_access_proxy(const_reference elt) const = 0;

    inline constexpr typename Hash::result_type hash_element(const key_type& key) const {
        return Hash{}(key) % capacity();
    }

    inline constexpr bool are_equal(const key_type& a, const key_type& b) const {
        return Equal{}(a, b);
    }

    iterator insert_into_next_free_after(typename Hash::result_type hash, T* elt) {
        bucket_t& free_spot = (bucket_t&)iterate_buckets_until(hash, [](const bucket_t& b) -> pair<bool, const bucket_t*> { return {!b.elt, &b}; });
        free_spot.elt.reset(elt);
        free_spot.state = CLEAN;
        return free_spot;
    }

    void rehash_if_needed(size_type num_buckets = 0) {
        bool implicit_resize = false;

        if (num_buckets == 0) {
            // ensure space for one more element
            num_buckets = m_size + 1;
            implicit_resize = true;
        }

        num_buckets++;  // reserve one bucket for 'end'

        if (m_buckets.capacity() < num_buckets) {
            // expand by a bit more than one to save allocations
            if (implicit_resize) num_buckets += 8;

            m_buckets.resize(num_buckets);
            for (size_type i = 0; i < m_buckets.capacity(); ++i)
                rehash_element(m_buckets[i]);

            for (size_type i = 0; i < m_buckets.capacity(); ++i)
                m_buckets[i].state = CLEAN;

            m_buckets.back().state = END;
        }
    }

    void rehash_element(bucket_t& bucket) {
        if (bucket.elt && bucket.state != REHASHED) {
            T* element = bucket.elt.release();
            bucket.state = REHASHED;
            auto new_hash = hash_element(key_access_proxy(*element));

            iterate_buckets_until(new_hash, [this, element](const bucket_t& b) -> pair<bool, const bucket_t*> {
                auto& bucket = (bucket_t&)b;
                rehash_element(bucket);
                if (!bucket.elt) {
                    bucket.elt.reset(element);
                    bucket.state = REHASHED;
                    return {true, &b};
                }
                return {false, nullptr};
            });
        }
    }

    template <typename UnaryPredicate>
    const bucket_t& iterate_buckets_until(typename Hash::result_type start, UnaryPredicate p) const {
        if (m_buckets.capacity() <= 1) return m_buckets.back();

        size_type cursor = start;
        do {
            pair<bool, const bucket_t*> res = p(m_buckets[cursor]);
            if (res.first) return res.second == nullptr ? m_buckets.back() : *res.second;
            cursor = (cursor + 1) % capacity();
        } while (cursor != start);
        return m_buckets.back();
    }

    const_iterator find_hash(const key_type& key, typename Hash::result_type hash) const {
        return (bucket_t&)iterate_buckets_until(hash, [key, this](const bucket_t& b) -> pair<bool, const bucket_t*> {
            if (b.state == DELETED) return {false, nullptr};
            if (!b.elt) return {true, nullptr};
            return {are_equal(key_access_proxy(*b.elt.get()), key), &b};
        });
    }

    bucket_t& find_first_used_bucket() const {
        return (bucket_t&)iterate_buckets_until(0, [](const bucket_t& b) -> pair<bool, const bucket_t*> {
            return {b.elt, &b};
        });
    }

    // FIXME: somehow honor allocator_type?
    vector<bucket_t> m_buckets{};
    size_type m_size = 0;
};

}  // namespace lw_std