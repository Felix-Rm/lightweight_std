
#pragma once

#define LWSTD_COMMON_CONTAINER_TYPES(Allocator) \
   public:                                      \
    class iterator;                             \
    using allocator_type = Allocator;           \
    using const_iterator = const iterator;

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
    using const_pointer = const pointer;
