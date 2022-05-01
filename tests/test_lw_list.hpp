#pragma once

#include <list>

#include "../list.hpp"
#include "ftest/test_logging.hpp"
#include "test_utilities.hpp"

class TestLwList : public ContainerTestDefaultMixin<TestLwList, lw_std::list, std::list> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(ContainerTestType::default_test_container_printer);
        tester.set_verify_container_printer(ContainerTestType::default_verify_container_printer);

        tester.add_grow_modifier("push_back", ContainerTestType::grow_by_push_back);
        tester.add_grow_modifier("push_front", ContainerTestType::grow_by_push_front);
        tester.add_grow_modifier("emplace", ContainerTestType::grow_by_emplace);
        tester.add_grow_modifier("emplace_back", ContainerTestType::grow_by_emplace_back);
        tester.add_grow_modifier("emplace_front", ContainerTestType::grow_by_emplace_front);
        tester.add_grow_modifier("insert", ContainerTestType::grow_by_insert);

        tester.add_shrink_modifier("pop_back", ContainerTestType::shrink_by_pop_back);
        tester.add_shrink_modifier("pop_front", ContainerTestType::shrink_by_pop_front);
        tester.add_shrink_modifier("erase", ContainerTestType::shrink_by_erase_by_iterator);

        tester.add_verifier("size", ContainerTestType::verify_size);
        tester.add_verifier("element position", ContainerTestType::verify_element_position_with_iterator);
        tester.add_verifier("find existing element", ContainerTestType::verify_find_existing_element);

        return tester.run_operations(operation_count);
    };
};