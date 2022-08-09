#pragma once

#include <utility>

/*
    This is a non default-constructable object with heap allocations to check if destructor is called aproproately
*/

class NonTrivial {
   public:
    NonTrivial() {
        m_data = new unsigned{-1u};
    }

    NonTrivial(unsigned a) {
        m_data = new unsigned{a};
    }

    // NonTrivial(const NonTrivial&) = delete;
    NonTrivial(const NonTrivial& other) {
        operator=(other);
    }

    NonTrivial(NonTrivial&& other) {
        operator=(std::move(other));
    }

    ~NonTrivial() {
        clear();
    }

    NonTrivial& operator=(const NonTrivial& other) {
        if (this != &other) {
            clear();
            if (other.m_data)
                m_data = new unsigned{*other.m_data};
        }
        return *this;
    }

    NonTrivial& operator=(NonTrivial&& other) {
        if (this != &other) {
            clear();
            m_data = other.m_data;
            other.m_data = nullptr;
        }
        return *this;
    }

    bool operator==(const NonTrivial& rhs) const {
        if (m_data == nullptr || rhs.m_data == nullptr) return false;
        return *m_data == *rhs.m_data;
    }

    bool operator!=(const NonTrivial& rhs) const {
        return !operator==(rhs);
    }

    unsigned data() const {
        return *m_data;
    }

    void clear() {
        if (m_data) {
            delete m_data;
            m_data = nullptr;
        }
    }

   private:
    unsigned* m_data{nullptr};
};
