#pragma once

template <typename it, typename T>
class iterator_impl {
   private:
    it& actual() {
        return *(it*)this;
    }

    const it& actual() const {
        return *(it*)this;
    }

   public:
    iterator_impl() = default;

    iterator_impl(const it& other) {
        actual().copy(other);
    }

    iterator_impl(it&& other) {
        actual().move(other);
    }

    it& operator=(const it& other) {
        if (&other != this)
            actual().copy(other);
        return *this;
    }

    it& operator=(it&& other) {
        if (&other != this)
            actual().move(other);
        return actual();
    }

    it& operator++() {
        actual().advance(1);
        return actual();
    }

    it operator++(int) {
        it tmp(actual());
        actual().advance(1);
        return tmp;
    }

    it operator+(int n) const {
        it tmp(actual());
        tmp.advance(n);
        return tmp;
    }

    bool operator==(const it& rhs) const {
        return ((it*)this)->equals(rhs);
    }

    bool operator!=(const it& rhs) const {
        return !operator==(rhs);
    }

    T& operator*() {
        return *actual().get();
    }

    T* operator->() {
        return actual().get();
    }

    const T& operator*() const {
        return *actual().get();
    };

    const T* operator->() const {
        return actual().get();
    };
};