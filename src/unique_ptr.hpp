#pragma once

#include "utility.hpp"

namespace lw_std {

template <typename T>
class unique_ptr {
   private:
    T* m_ptr{nullptr};

   public:
    unique_ptr() = default;

    unique_ptr(T* ptr)
        : m_ptr(ptr){};

    unique_ptr(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& other) {
        operator=(lw_std::move(other));
    }

    ~unique_ptr() {
        destroy();
    }

    operator bool() const {
        return m_ptr != nullptr;
    }

    unique_ptr& operator=(unique_ptr&& other) {
        if (this != &other) {
            if (m_ptr) delete m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    unique_ptr& operator=(T* ptr) {
        if (m_ptr) delete m_ptr;
        m_ptr = ptr;

        return *this;
    }

    T& operator->() {
        return *m_ptr;
    }

    const T& operator->() const {
        return *m_ptr;
    }

    T& operator*() {
        return *m_ptr;
    }

    const T& operator*() const {
        return *m_ptr;
    }

    T* get() {
        return m_ptr;
    }

    const T* get() const {
        return m_ptr;
    }

    void reset(T* new_obj) {
        operator=(new_obj);
    }

    T* release() {
        auto tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    void destroy() {
        if (m_ptr) {
            delete m_ptr;
            m_ptr = nullptr;
        };
    }
};
}  // namespace lw_std
