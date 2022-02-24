#pragma once

#include <vector>

#include "ftest/test_logging.hpp"
#include "test_utilities.hpp"
#include "vector.hpp"

class TestLwVector : public ContainerTestDefaultMixin<TestLwVector, lw_std::vector, std::vector> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(ContainerTestType::default_test_container_printer);
        tester.set_verify_container_printer(ContainerTestType::default_verify_container_printer);

        tester.add_grow_modifier("push_back", ContainerTestType::default_grow_by_push_back);
        tester.add_grow_modifier("emplace_back", ContainerTestType::default_grow_by_emplace_back);
        tester.add_grow_modifier("emplace", ContainerTestType::default_grow_by_emplace);
        tester.add_grow_modifier("insert", ContainerTestType::default_grow_by_insert);

        tester.add_shrink_modifier("pop_back", ContainerTestType::default_shrink_by_pop_back);
        tester.add_shrink_modifier("erase", ContainerTestType::default_shrink_by_erase_by_iterator);

        tester.add_verifier("size", ContainerTestType::default_verify_size);
        tester.add_verifier("element position", ContainerTestType::default_verify_element_position_with_at);
        tester.add_verifier("find existing element", ContainerTestType::default_verify_find_existing_element);

        return tester.run_operations(operation_count);
    };
};