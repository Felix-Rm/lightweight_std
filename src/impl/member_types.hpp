
#pragma once

#define LWSTD_COMMON_CONTAINER_TYPES(T, IT_T, Allocator)                          \
   private:                                                                       \
    template <typename P, typename IT_P>                                          \
    class iterator_def;                                                           \
                                                                                  \
   public:                                                                        \
    using allocator_type = Allocator;                                             \
    using iterator = iterator_impl<iterator_def<T, IT_T>, iterator_def<T, IT_T>>; \
    using const_iterator = iterator_impl<iterator_def<const T, const IT_T>, iterator>;

#define LWSTD_COMMON_VALUE_TYPES(T)    \
   public:                             \
    using value_type = T;              \
    using size_type = size_t;          \
    using difference_type = ptrdiff_t; \
    using reference = value_type&;     \
    using const_reference = const value_type&;

#define LWSTD_COMMON_POINTER_TYPES(T) \
   public:                            \
    using pointer = T*;               \
    using const_pointer = const T*;
