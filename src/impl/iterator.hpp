#pragma once

namespace lw_std {

template <typename it, typename non_const_it>
class iterator_impl : public it {
   public:
    typedef it underlying_type;

    constexpr iterator_impl(const typename it::data_type& data)
        : it(data) {}

    constexpr iterator_impl(const non_const_it& other)
        : it(other) {}

    constexpr iterator_impl(const iterator_impl& other)
        : it(static_cast<const it&>(other)) {}

    constexpr iterator_impl(iterator_impl&& other)
        : it(static_cast<const it&&>(other)) {}

    constexpr iterator_impl& operator=(const iterator_impl& other) {
        if (&other != this)
            it::operator=(static_cast<const it&>(other));
        return *this;
    }

    constexpr iterator_impl& operator=(iterator_impl&& other) {
        if (&other != this)
            it::operator=(static_cast<it&&>(other));
        return *this;
    }

    constexpr iterator_impl& operator++() {
        it::advance(1);
        return *this;
    }

    constexpr iterator_impl operator++(int) {
        iterator_impl tmp(*this);
        it::advance(1);
        return tmp;
    }

    constexpr iterator_impl operator+(int n) const {
        iterator_impl tmp(*this);
        tmp.advance(n);
        return tmp;
    }

    [[nodiscard]] constexpr bool operator==(const iterator_impl& rhs) const {
        return it::equal(static_cast<const it&>(rhs));
    }

    [[nodiscard]] constexpr bool operator!=(const iterator_impl& rhs) const {
        return !it::equal(static_cast<const it&>(rhs));
    }

    [[nodiscard]] constexpr auto& operator*() {
        return *it::get();
    }

    [[nodiscard]] constexpr auto* operator->() {
        return it::get();
    }

    [[nodiscard]] constexpr const auto& operator*() const {
        return *it::get();
    }

    [[nodiscard]] constexpr const auto* operator->() const {
        return it::get();
    }
};

}  // namespace lw_std