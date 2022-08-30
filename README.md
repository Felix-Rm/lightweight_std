# lightweight_std

[![build & test](https://github.com/Felix-Rm/lightweight_std/actions/workflows/cmake.yml/badge.svg)](https://github.com/Felix-Rm/lightweight_std/actions/workflows/cmake.yml)
[![Sonar Cube Static Analysis](https://sonarcloud.io/api/project_badges/measure?project=Felix-Rm_lightweight_std&metric=ncloc)](https://sonarcloud.io/dashboard?id=Felix-Rm_lightweight_std)
[![Sonar Cube Static Analysis](https://sonarcloud.io/api/project_badges/measure?project=Felix-Rm_lightweight_std&metric=alert_status)](https://sonarcloud.io/dashboard?id=Felix-Rm_lightweight_std)


## About

Lightweight_std is a library which provides some features of the c++ standard library. It is meant for use on embedded systems like microcontrollers, where the standard library is to memory intensive or just not available.

The current version includes implementations for:
- \<algorithm> (in "algorithm.hpp")
    - synthesized comparison operators (all comparison operators expressed in terms of less than)
    - `std::find`
    - `std::find_if`
    - `std::find_if_not`
    - common version of free `std::erase`
    - common version of free `std::erase_if`
    - `std::remove`
    - `std::remove_if`
    - `std::max` (also available as `max_of` for Arduino environments)
    - `std::min` (also available as `min_of` for Arduino environments)
    - `std::equal`
    - `std::lexicographical_compare`

- \<functional> (in "functional.hpp")
    - `std::equal_to`
    - `std::hash` (with specialization for integral types and `lw_std::string`)

- \<limits> (in "limits.hpp")
    - `std::limits` (just `::max` and `::min`) (with specialization  for `uint8_t`, `uint16_t`, `uint32_t` and `uint64_t`)

- \<list> (in "list.hpp")
    - `std::list` (non-complete API)

- \<memory> (in "memory.hpp")
    - `std::allocator`
    - `std::unique_ptr` (non-complete API)

- \<queue> (in "queue.hpp")
    - `std::queue` (with `list` as default underlying container) (non-complete API)

- \<string> (in "string.hpp")
    - `std::string` (passthrough of `std::string` or Arduino's `String`)

- \<unordered_set> (in "unordered_set.hpp")
    - `std::unordered_set` (non-complete API)

- \<unordered_map> (in "unordered_map.hpp")
    - `std::unordered_map` (non-complete API)

- \<utility> (in "utility.hpp")
    - `std::move`
    - `std::forward`
    - `std::swap`
    - `std::pair` (with compatibility constructor and comparison operators for `std::pair`, enable by defining LWSTD_BUILD_STD_COMPATIBILITY)
    - `std::make_pair`

- \<vector> (in "vector.hpp")
    - `std::vector` (non-complete API)


