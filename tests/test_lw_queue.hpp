#pragma once

#include <queue>

#include "compile_accelerators/accelerator_defs.hpp"
#include "container_test_mixin.hpp"
#include "queue.hpp"

class TestLwQueue : public ContainerTestDefaultMixin<TestLwQueue, lw_std::queue, std::queue> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename Container>
    static void printer(const Container&) {
    }

    template <typename ContainerTestType>
    static TestLogging::test_result run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_test_container_size_getter(ContainerTestType::default_test_container_size_getter);
        tester.set_verify_container_size_getter(ContainerTestType::default_verify_container_size_getter);

        tester.set_test_container_printer(printer<typename ContainerTestType::test_container_t>);
        tester.set_verify_container_printer(printer<typename ContainerTestType::verify_container_t>);

        tester.add_grow_modifier("push", ContainerTestType::grow_by_push);
        tester.add_grow_modifier("emplace", ContainerTestType::grow_by_emplace_no_pos);

        tester.add_shrink_modifier("pop_back", ContainerTestType::shrink_by_pop);

        tester.add_verifier("size", ContainerTestType::verify_size);
        tester.add_verifier("front element position", ContainerTestType::verify_front_element_position);
        tester.add_verifier("back element position", ContainerTestType::verify_back_element_position);

        return tester.run_operations(operation_count);
    }
};

extern LWSTD_TEST_ACCELERATE(TestLwQueue, queue, int);
extern LWSTD_TEST_ACCELERATE(TestLwQueue, queue, NonTrivial);