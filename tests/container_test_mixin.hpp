
#pragma once

#include <ftest/test_logging.hpp>

#include "container_tester.hpp"
#include "non_trivial.hpp"

template <typename Derived, template <typename...> class test_container_t, template <typename...> class verify_container_t>
class ContainerTestDefaultMixin {
   public:
    static TestLogging::test_result run_with_int(size_t operation_count) {
        typedef ContainerTester<test_container_t<int>, verify_container_t<int>> tester_t;
        tester_t tester;

        tester.set_value_generator(tester_t::default_uint_generator);

        return Derived::run_templated(tester, operation_count);
    }

    static TestLogging::test_result run_with_non_trivial(size_t operation_count) {
        typedef ContainerTester<test_container_t<NonTrivial>, verify_container_t<NonTrivial>> tester_t;
        tester_t tester;

        tester.set_value_generator(tester_t::default_non_trivial_generator);

        return Derived::run_templated(tester, operation_count);
    }

    static TestLogging::test_result run_with_int_int(size_t operation_count) {
        typedef ContainerTester<test_container_t<int, int>, verify_container_t<int, int>> tester_t;
        tester_t tester;

        tester.set_value_generator([]() -> std::pair<const int, int> {
            return {tester_t::default_uint_generator(), tester_t::default_uint_generator()};
        });

        return Derived::run_templated(tester, operation_count);
    }

    static TestLogging::test_result run_with_int_non_trivial(size_t operation_count) {
        typedef ContainerTester<test_container_t<int, NonTrivial>, verify_container_t<int, NonTrivial>> tester_t;
        tester_t tester;

        tester.set_value_generator([]() -> std::pair<const int, NonTrivial> {
            return {tester_t::default_uint_generator(), tester_t::default_non_trivial_generator()};
        });

        return Derived::run_templated(tester, operation_count);
    }
};