# IAC

[![build & test](https://github.com/Felix-Rm/lightweight_std/actions/workflows/cmake.yml/badge.svg)](https://github.com/Felix-Rm/lightweight_std/actions/workflows/cmake.yml)
[![Sonar Cube Static Analysis](https://sonarcloud.io/api/project_badges/measure?project=Felix-Rm_lightweight_std&metric=ncloc)](https://sonarcloud.io/dashboard?id=Felix-Rm_lightweight_std)
[![Sonar Cube Static Analysis](https://sonarcloud.io/api/project_badges/measure?project=Felix-Rm_lightweight_std&metric=alert_status)](https://sonarcloud.io/dashboard?id=Felix-Rm_lightweight_std)


## About

Lightweight_std is a library which provides some features of the c++ standard library. It is meant for use on embedded systems like microcontrollers, where the standard library is to memory intensive or just not available.

The current version includes implementations for:
- \<utility> (in "utility.hpp")
    - `std::swap`
    - `std::move`
    - `std::pair`
    - `std::make_pair`
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
- \<limits> (in "limits.hpp")
    - `std::limits` (for `uint8_t` and `uint16_t`)
- \<memory> (in "unique_ptr.hpp")
    - `std::unique_ptr`
- \<string> (in "string.hpp")
    - `std::string` (passthrough of `std::string` or Arduino's String)
- \<vector> (in "vector.hpp")
    - `std::vector` (all members except `data` and those requiring `allocator`, `initializer_list`, or `reverse_iterator`)
- \<list> (in "list.hpp")
    - `std::list` (no complete interface)
- \<queue> (in "queue.hpp")
    - `std::queue` (with `list` as underlying container)
- \<unordered_set> (in "unordered_set.hpp")
    - `std::unordered_set` (with `list` as underlying container -> **bad time-complexity on any operation requiring lookup**)
- \<unordered_map> (in "unordered_map.hpp")
    - `std::unordered_map` (with `list` as underlying container -> **bad time-complexity on any operation requiring lookup**)