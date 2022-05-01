#pragma once

#include <vector>

#include "../vector.hpp"
#include "ftest/test_logging.hpp"
#include "test_utilities.hpp"

class TestLwVector : public ContainerTestDefaultMixin<TestLwVector, lw_std::vector, std::vector> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(ContainerTestType::default_test_container_printer);
        tester.set_verify_container_printer(ContainerTestType::default_verify_container_printer);

        tester.add_neutral_modifier("operator=(const T&)", ContainerTestType::modify_by_copy);
        tester.add_neutral_modifier("operator=(T&&)", ContainerTestType::modify_by_move);

        tester.add_neutral_modifier("assign", ContainerTestType::modify_by_assign);
        tester.add_neutral_modifier("assign (range)", ContainerTestType::modify_by_assign_range);

        tester.add_verifier("element position (at)", ContainerTestType::verify_element_position_with_at);
        tester.add_verifier("element position (operator[])", ContainerTestType::verify_element_position_with_operator_brackets);

        tester.add_verifier("front", ContainerTestType::verify_front);
        tester.add_verifier("back", ContainerTestType::verify_back);

        tester.add_verifier("element position (iterator)", ContainerTestType::verify_element_position_with_iterator);

        (void)tester.tc().empty();  // just test if it compiles
        tester.add_verifier("size", ContainerTestType::verify_size);
        (void)tester.tc().max_size();  // just test if it compiles

        tester.add_neutral_modifier("reserve", ContainerTestType::modify_by_reserve);

        (void)tester.tc().capacity();  // just test if it compiles

        tester.add_neutral_modifier("shrink_to_fit", ContainerTestType::modify_by_shrink_to_fit);
        tester.add_shrink_modifier("clear", ContainerTestType::shrink_by_clear);

        tester.add_grow_modifier("insert", ContainerTestType::grow_by_insert);
        tester.add_grow_modifier("insert (rvalue)", ContainerTestType::grow_by_insert_rvalue);
        tester.add_grow_modifier("insert (count)", ContainerTestType::grow_by_insert_count);
        tester.add_grow_modifier("insert (range)", ContainerTestType::grow_by_insert_range);

        tester.add_grow_modifier("emplace", ContainerTestType::grow_by_emplace);

        tester.add_shrink_modifier("erase", ContainerTestType::shrink_by_erase_by_iterator);
        tester.add_shrink_modifier("erase (range)", ContainerTestType::shrink_by_erase_by_range);

        tester.add_grow_modifier("push_back", ContainerTestType::grow_by_push_back);
        tester.add_grow_modifier("push_back (rvalue)", ContainerTestType::grow_by_push_back_rvalue);

        tester.add_grow_modifier("emplace_back", ContainerTestType::grow_by_emplace_back);

        tester.add_shrink_modifier("pop_back", ContainerTestType::shrink_by_pop_back);

        tester.add_neutral_modifier("resize", ContainerTestType::modify_by_resize);
        tester.add_neutral_modifier("resize (value)", ContainerTestType::modify_by_resize_with_value);

        tester.add_neutral_modifier("swap", ContainerTestType::modify_by_swap);

        return tester.run_operations(operation_count);
    };
};