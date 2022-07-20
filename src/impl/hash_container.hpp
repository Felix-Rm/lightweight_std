#pragma once

#include "../functional.hpp"
#include "../vector.hpp"
#include "iterator.hpp"
#include "member_types.hpp"

namespace lw_std {

template <typename Derived, typename T, typename KeyType, typename Hash = hash<KeyType>, typename Equal = equal_to<KeyType>, typename Allocator = allocator<T>>
class hash_container_impl {
   protected:
    enum bucket_state {
        CLEAN,
        DELETED,
        REHASHED,
        END
    };

    struct bucket_t {
        unique_ptr<T> elt{};
        bucket_state state{CLEAN};

        constexpr bucket_t() = default;

        constexpr bucket_t(const bucket_t& other) {
            operator=(other);
        }

        constexpr bucket_t(bucket_t&& other) {
            operator=(lw_std::move(other));
        }

        constexpr bucket_t& operator=(const bucket_t& other) {
            if (other.elt)
                elt = make_unique<T>(*other.elt);
            state = other.state;
            return *this;
        }

        constexpr bucket_t& operator=(bucket_t&& other) {
            elt = lw_std::move(other.elt);
            state = other.state;
            return *this;
        }
    };

    typedef typename Allocator::template rebind<bucket_t>::other bucket_allocator_t;

   public:
    /*
        MEMBER TYPES
    */

    LWSTD_COMMON_CONTAINER_TYPES(T, bucket_t*, Allocator)
    LWSTD_COMMON_VALUE_TYPES(T)

    using key_type = KeyType;
    using key_equal = Equal;
    using hasher = Hash;

    /*
        MEMBER FUNCTIONS
    */

    // FIXME: (constructor) (1) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set
    constexpr hash_container_impl() = default;

    // FIXME: (constructor) (2) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // (constructor) (3) https://en.cppreference.com/w/cnoexceptpp/container/unordered_set/unordered_set
    constexpr hash_container_impl(const hash_container_impl& other) {
        operator=(other);
    }

    // FIXME: (constructor) (3) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set
    constexpr hash_container_impl(hash_container_impl&& other) {
        operator=(lw_std::move(other));
    }

    // FIXME: (constructor) (4) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // FIXME: (constructor) (5) https://en.cppreference.com/w/cpp/container/unordered_set/unordered_set

    // (destructor) https://en.cppreference.com/w/cpp/container/unordered_set/~unordered_set
    ~hash_container_impl() = default;

    // operator= (1) https://en.cppreference.com/w/cpp/container/unordered_map/operator%3D
    constexpr hash_container_impl& operator=(const hash_container_impl& other) = default;

    // operator= (2) https://en.cppreference.com/w/cpp/container/unordered_map/operator%3D
    constexpr hash_container_impl& operator=(hash_container_impl&& other) = default;

    // FIXME: operator= (3) https://en.cppreference.com/w/cpp/container/unordered_map/operator%3D

    // get_allocator https://en.cppreference.com/w/cpp/container/unordered_set/get_allocator
    [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
        return allocator_type(m_buckets.get_allocator());
    }

    /*
        Iterators
    */

    // begin https://en.cppreference.com/w/cpp/container/unordered_set/begin
    [[nodiscard]] constexpr iterator begin() noexcept {
        return &find_first_used_bucket(*this);
    }

    // begin https://en.cppreference.com/w/cpp/container/unordered_set/begin
    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return &find_first_used_bucket(*this);
    }

    // cbegin https://en.cppreference.com/w/cpp/container/unordered_set/begin
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return &find_first_used_bucket(*this);
    }

    // end https://en.cppreference.com/w/cpp/container/unordered_set/end
    [[nodiscard]] constexpr iterator end() noexcept {
        return &m_buckets.back();
    }

    // end https://en.cppreference.com/w/cpp/container/unordered_set/end
    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return &m_buckets.back();
    }

    // cend https://en.cppreference.com/w/cpp/container/unordered_set/end
    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return &m_buckets.back();
    }

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

    /*
        Modifiers
    */

    // clear https://en.cppreference.com/w/cpp/container/unordered_set/clear
    constexpr void clear() noexcept {
        m_buckets.resize(0);
        m_size = 0;
    }

    // insert (1) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    constexpr pair<iterator, bool> insert(const_reference value) {
        return emplace(lw_std::move(value));
    }

    // insert (2) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    constexpr pair<iterator, bool> insert(T&& value) {
        return emplace(lw_std::move(value));
    }

    // FIXME: insert (3) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    // FIXME: insert (4) https://en.cppreference.com/w/cpp/container/unordered_set/insert

    // insert (5) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    template <typename InputIt>
    constexpr void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            emplace(*first);
        }
    }

    // FIXME: insert (6) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    // FIXME: insert (7) https://en.cppreference.com/w/cpp/container/unordered_set/insert
    // FIXME: insert (8) https://en.cppreference.com/w/cpp/container/unordered_set/insert

    // emplace https://en.cppreference.com/w/cpp/container/unordered_set/emplace
    template <class... Args>
    constexpr pair<iterator, bool> emplace(Args&&... args) {
        // NOTE: this will make sure the underlying vector has capacity > 0;
        //       if capacity is zero we would get division by zero in hash
        rehash_if_needed();

        // FIXME: use allocator
        unique_ptr<T> element = make_unique<T>(lw_std::forward<Args>(args)...);
        auto hash = hash_element(key_access_proxy(*element));
        iterator res = &find_hash(*this, key_access_proxy(*element), hash);

        if (res == end()) {
            m_size++;
            return {insert_into_next_free_after(hash, element.release()), true};
        }

        return {res, false};
    }

    // FIXME: emplace_hint https://en.cppreference.com/w/cpp/container/unordered_set/emplace_hint

    // erase (1) https://en.cppreference.com/w/cpp/container/unordered_set/erase
    constexpr iterator erase(const_iterator pos) {
        const auto& underlying_pos = static_cast<typename const_iterator::underlying_type>(pos);
        auto* mutable_bucket = const_cast<bucket_t*>(underlying_pos.m_data);
        mutable_bucket->elt.reset();
        mutable_bucket->state = DELETED;
        m_size--;
        return iterator(mutable_bucket) + 1;
    }

    // erase (2) https://en.cppreference.com/w/cpp/container/unordered_set/erase
    constexpr iterator erase(const_iterator first, const_iterator last) {
        iterator pos;
        for (iterator it = first; it != last; it++)
            pos = erase(it);
        return pos;
    }

    // erase (3) https://en.cppreference.com/w/cpp/container/unordered_set/erase
    constexpr size_type erase(key_type key) {
        const_iterator res = &find_hash(*this, key, hash_element(key));
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
    [[nodiscard]] constexpr size_type count(const key_type& key) const {
        return find(key) == end() ? 0 : 1;
    }

    // FIXME: count (2) https://en.cppreference.com/w/cpp/container/unordered_set/count

    // find (1) https://en.cppreference.com/w/cpp/container/unordered_set/find
    [[nodiscard]] constexpr iterator find(const key_type& key) {
        return &find_hash(*this, key, hash_element(key));
    }

    // find (2) https://en.cppreference.com/w/cpp/container/unordered_set/find
    [[nodiscard]] constexpr const_iterator find(const key_type& key) const {
        return &find_hash(*this, key, hash_element(key));
    }

    // FIXME: find (3) https://en.cppreference.com/w/cpp/container/unordered_set/find
    // FIXME: find (4) https://en.cppreference.com/w/cpp/container/unordered_set/find

    // contains (1) https://en.cppreference.com/w/cpp/container/unordered_set/contains
    [[nodiscard]] constexpr bool contains(const key_type& key) const {
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
    [[nodiscard]] constexpr size_type bucket_count() const {
        return m_buckets.capacity();
    }

    // max_bucket_count https://en.cppreference.com/w/cpp/container/unordered_set/max_bucket_count
    [[nodiscard]] constexpr size_type max_bucket_count() const {
        return m_buckets.max_size();
    }

    // bucket_size https://en.cppreference.com/w/cpp/container/unordered_set/bucket_size
    [[nodiscard]] constexpr size_type bucket_size(size_type n) const {
        return m_buckets[n].elt ? 1 : 0;
    }

    // bucket_size https://en.cppreference.com/w/cpp/container/unordered_set/bucket_size
    [[nodiscard]] constexpr size_type bucket(const key_type& key) const {
        return hash_element(key);
    }

    /*
        Hash policy
    */

    // FIXME: load_factor https://en.cppreference.com/w/cpp/container/unordered_set/load_factor

    // FIXME: max_load_factor https://en.cppreference.com/w/cpp/container/unordered_set/max_load_factor

    // rehash https://en.cppreference.com/w/cpp/container/unordered_set/rehash
    constexpr void rehash(size_type count) {
        rehash_if_needed(count);
    }

    // reserve https://en.cppreference.com/w/cpp/container/unordered_set/reserve
    constexpr void reserve(size_type count) {
        rehash_if_needed(count);
    }

    /*
        Observers
    */

    // FIXME: hash_function https://en.cppreference.com/w/cpp/container/unordered_set/hash_function

    // FIXME: key_eq https://en.cppreference.com/w/cpp/container/unordered_set/key_eq

   protected:
    [[nodiscard]] constexpr const key_type& key_access_proxy(const_reference elt) const {
        return Derived::key_access_proxy(elt);
    }

    template <typename This>
    [[nodiscard]] static constexpr auto& find_hash(This& obj, const key_type& key, typename Hash::result_type hash) {
        typedef decltype(obj.m_buckets.back()) bucket_ref_type;
        typedef decltype(&obj.m_buckets.back()) bucket_ptr_type;

        auto obj_ptr = &obj;

        return iterate_buckets_until(obj, hash, [key, obj_ptr](bucket_ref_type b) -> pair<bool, bucket_ptr_type> {
            if (b.state == DELETED) return {false, nullptr};
            if (!b.elt) return {true, nullptr};
            return {obj_ptr->are_equal(obj_ptr->key_access_proxy(*b.elt.get()), key), &b};
        });
    }

    [[nodiscard]] constexpr typename Hash::result_type hash_element(const key_type& key) const {
        return Hash{}(key) % capacity();
    }

    [[nodiscard]] constexpr bool are_equal(const key_type& a, const key_type& b) const {
        return Equal{}(a, b);
    }

   private:
    template <typename P, typename IT_P>
    class iterator_def {
        friend hash_container_impl;

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
            return m_data->elt.get();
        }

        [[nodiscard]] constexpr const P* get() const {
            return m_data->elt.get();
        }

        constexpr void advance(int n) {
            for (int i = 0; i < n; ++i)
                do {
                    if (m_data->state == END) return;
                    m_data++;
                } while (!m_data->elt);
        }

       private:
        IT_P m_data{nullptr};
    };

    [[nodiscard]] constexpr size_type capacity() const {
        return m_buckets.capacity() - 1;  // end bucket not included
    }

    constexpr iterator insert_into_next_free_after(typename Hash::result_type hash, T* elt) {
        bucket_t& free_spot = iterate_buckets_until(*this, hash, [](bucket_t& b) -> pair<bool, bucket_t*> { return {!b.elt, &b}; });
        free_spot.elt.reset(elt);
        free_spot.state = CLEAN;
        return &free_spot;
    }

    constexpr void rehash_if_needed(size_type num_buckets = 0) {
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

    constexpr void rehash_element(bucket_t& bucket) {
        if (bucket.elt && bucket.state != REHASHED) {
            T* element = bucket.elt.release();
            bucket.state = REHASHED;
            auto new_hash = hash_element(key_access_proxy(*element));

            iterate_buckets_until(*this, new_hash, [this, element](bucket_t& b) -> pair<bool, bucket_t*> {
                rehash_element(b);
                if (!b.elt) {
                    b.elt.reset(element);
                    b.state = REHASHED;
                    return {true, &b};
                }
                return {false, nullptr};
            });
        }
    }

    template <typename This>
    [[nodiscard]] static constexpr auto& find_first_used_bucket(This& obj) {
        typedef decltype(obj.m_buckets.back()) bucket_ref_type;
        typedef decltype(&obj.m_buckets.back()) bucket_ptr_type;

        return iterate_buckets_until(obj, 0, [](bucket_ref_type b) -> pair<bool, bucket_ptr_type> {
            return {b.elt, &b};
        });
    }

    template <typename This, typename UnaryPredicate>
    static constexpr auto& iterate_buckets_until(This& obj, typename Hash::result_type start, UnaryPredicate p) {
        typedef decltype(&obj.m_buckets.back()) bucket_ptr_type;

        if (obj.m_buckets.capacity() <= 1) return obj.m_buckets.back();

        size_type cursor = start;
        do {
            pair<bool, bucket_ptr_type> res = p(obj.m_buckets[cursor]);
            if (res.first) return res.second == nullptr ? obj.m_buckets.back() : *res.second;
            cursor = (cursor + 1) % obj.capacity();
        } while (cursor != start);

        return obj.m_buckets.back();
    }

    vector<bucket_t, bucket_allocator_t> m_buckets{};
    size_type m_size = 0;
};

}  // namespace lw_std