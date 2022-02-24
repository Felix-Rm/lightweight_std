#pragma once

#include <list>

#include "ftest/test_logging.hpp"
#include "list.hpp"
#include "test_utilities.hpp"

class TestLwList : public ContainerTestDefaultMixin<TestLwList, lw_std::list, std::list> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(ContainerTestType::default_test_container_printer);
        tester.set_verify_container_printer(ContainerTestType::default_verify_container_printer);

        tester.add_grow_modifier("push_back", ContainerTestType::default_grow_by_push_back);
        tester.add_grow_modifier("push_front", ContainerTestType::default_grow_by_push_front);
        tester.add_grow_modifier("emplace", ContainerTestType::default_grow_by_emplace);
        tester.add_grow_modifier("emplace_back", ContainerTestType::default_grow_by_emplace_back);
        tester.add_grow_modifier("emplace_front", ContainerTestType::default_grow_by_emplace_front);
        tester.add_grow_modifier("insert", ContainerTestType::default_grow_by_insert);

        tester.add_shrink_modifier("pop_back", ContainerTestType::default_shrink_by_pop_back);
        tester.add_shrink_modifier("pop_front", ContainerTestType::default_shrink_by_pop_front);
        tester.add_shrink_modifier("erase", ContainerTestType::default_shrink_by_erase_by_iterator);

        tester.add_verifier("size", ContainerTestType::default_verify_size);
        tester.add_verifier("element position", ContainerTestType::default_verify_element_position);
        tester.add_verifier("find existing element", ContainerTestType::default_verify_find_existing_element);

        return tester.run_operations(operation_count);
    };
};