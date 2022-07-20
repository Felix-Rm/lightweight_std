#pragma once

#include <unordered_map>

#include "ftest/test_logging.hpp"
#include "test_utilities.hpp"
#include "unordered_map.hpp"

class TestLwUnorderedMap : public ContainerTestDefaultMixin<TestLwUnorderedMap, lw_std::unordered_map, std::unordered_map> {
    friend ContainerTestDefaultMixin;

   private:
    template <typename ContainerTestType>
    static TestLogging::test_result_t run_templated(ContainerTestType& tester, size_t operation_count) {
        tester.set_size_getter(ContainerTestType::default_size_getter);

        tester.set_test_container_printer(ContainerTestType::default_test_container_printer);
        tester.set_verify_container_printer(ContainerTestType::default_verify_container_printer);

        tester.add_neutral_modifier("operator=(const T&)", ContainerTestType::modify_by_copy);
        tester.add_neutral_modifier("operator=(T&&)", ContainerTestType::modify_by_move);

        tester.add_verifier("element position (at)", ContainerTestType::verify_element_position_with_at_for_map);
        tester.add_verifier("element position (operator[])", ContainerTestType::verify_element_position_with_operator_brackets_for_map);

        (void)tester.tc().empty();  // just test if it compiles
        tester.add_verifier("size", ContainerTestType::verify_size);
        (void)tester.tc().max_size();  // just test if it compiles

        tester.add_shrink_modifier("clear", ContainerTestType::shrink_by_clear);

        tester.add_grow_modifier("insert", ContainerTestType::grow_by_insert_no_pos);
        tester.add_grow_modifier("insert (rvalue)", ContainerTestType::grow_by_insert_rvalue_no_pos);
        // FIXME: tester.add_grow_modifier("insert (range)", ContainerTestType::grow_by_insert_range_no_pos);

        tester.add_grow_modifier("emplace", ContainerTestType::grow_by_emplace_no_pos);

        tester.add_shrink_modifier("erase", ContainerTestType::shrink_by_erase_by_iterator_for_map);
        // FIXME: tester.add_shrink_modifier("erase (range)", ContainerTestType::shrink_by_erase_by_range_for_map);
        tester.add_shrink_modifier("erase (value)", ContainerTestType::shrink_by_erase_by_value_for_map);

        (void)tester.tc().count(0);  // just test if it compiles
        tester.add_verifier("find (existing)", ContainerTestType::verify_find_existing_element_for_map);
        tester.add_verifier("find (element inclusion)", ContainerTestType::verify_element_inclusion_for_map);
        (void)tester.tc().contains(0);  // just test if it compiles

        (void)tester.tc().bucket_count();      // just test if it compiles
        (void)tester.tc().max_bucket_count();  // just test if it compiles
        // FIXME: (void) tester.tc().bucket_size(0);      // just test if it compiles
        (void)tester.tc().bucket(0);  // just test if it compiles

        (void)tester.tc().rehash(0);  // just test if it compiles
        tester.add_neutral_modifier("reserve", ContainerTestType::modify_by_reserve);

        return tester.run_operations(operation_count);
    }
};