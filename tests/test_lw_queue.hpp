#pragma once

#include <queue>

#include "ftest/test_logging.hpp"
#include "queue.hpp"
#include "test_utilities.hpp"

class TestLwQueue : public ContainerTestDefaultMixin<TestLwQueue, lw_std::queue, std::queue> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename Container>
    static void printer(const Container& c) {
    }

    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(printer<typename ContainerTestType::test_container_t>);
        tester.set_verify_container_printer(printer<typename ContainerTestType::verify_container_t>);

        tester.add_grow_modifier("push", ContainerTestType::default_grow_by_push);
        tester.add_grow_modifier("emplace", ContainerTestType::default_grow_by_emplace_no_iterator);

        tester.add_shrink_modifier("pop_back", ContainerTestType::default_shrink_by_pop);

        tester.add_verifier("size", ContainerTestType::default_verify_size);
        tester.add_verifier("front element position", ContainerTestType::default_verify_front_element_position);
        tester.add_verifier("back element position", ContainerTestType::default_verify_back_element_position);

        return tester.run_operations(operation_count);
    };
};