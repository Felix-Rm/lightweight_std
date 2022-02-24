#pragma once

#include <unordered_set>

#include "ftest/test_logging.hpp"
#include "test_utilities.hpp"
#include "unordered_set.hpp"

class TestLwUnorderedSet : public ContainerTestDefaultMixin<TestLwUnorderedSet, lw_std::unordered_set, std::unordered_set> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(ContainerTestType::default_test_container_printer);
        tester.set_verify_container_printer(ContainerTestType::default_verify_container_printer);

        tester.add_grow_modifier("insert", ContainerTestType::default_grow_by_insert_no_iterator);
        tester.add_grow_modifier("emplace", ContainerTestType::default_grow_by_emplace_no_iterator);

        tester.add_shrink_modifier("erase", ContainerTestType::default_shrink_by_erase_by_value);

        tester.add_verifier("size", ContainerTestType::default_verify_size);
        tester.add_verifier("element inclusion", ContainerTestType::default_verify_element_inclusion);

        return tester.run_operations(operation_count);
    };
};